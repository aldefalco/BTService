#include "stdafx.h"
#include "log.h"
#include "persistence.h"

namespace BTService { namespace Server {

	Persistence::Persistence(int timeout_, Task task_):
		timeout(timeout_),
		task(task_)
	{
		LDEBUG() << "New persistence instance";
	}

	Persistence::~Persistence()
	{
		LDEBUG() << "Persistence destroying";
		close_stream();
	}

	void Persistence::start()
	{
		LDEBUG() << "Persistence starting...";
		stopped.lock();
		thread = std::thread(&Persistence::_task_template, this);
		LTRACE() << "Persistence started";
	}

	void Persistence::stop()
	{
		LDEBUG() << "Persistence stopping...";
		stopped.unlock();
		thread.join();
		LTRACE() << "Persistence stopped";
	}

	void Persistence::close_stream()
	{
	}

	void Persistence::_task_template()
	{
		LDEBUG() << "Persistence _task_template";
		using sec = std::chrono::seconds;
		while (!stopped.try_lock_for(sec(timeout)))
		{
			LTRACE() << "Persistence task starting....";
			std::ostream & s = open_stream();
			LTRACE() << "Persistence stream opened";
			task(s);
			LTRACE() << "Persistence task completed";
			close_stream();
		} 
	}

	PersistenceFile::PersistenceFile(const std::string & file_, int timeout_, Task task_):
		Persistence(timeout_, task_),
		file(file_)
	{
		LDEBUG() << "Persistence for file instance creating...";
		stream = std::ofstream(file, std::ios_base::binary);
	}

	std::ostream & Server::PersistenceFile::open_stream()
	{
		LDEBUG() << "Persistence for file stream opening...";
		stream.open(file, std::ios_base::binary);
		LTRACE() << "Stream was opened";
		return stream;
	}

	void PersistenceFile::close_stream()
	{
		LDEBUG() << "Persistence for file stream closing...";
		stream.close();
		LTRACE() << "Stream was closed";
	}

	PersistenceString::PersistenceString(int timeout_, Task task_, Complete complete_) :
		Persistence(timeout_, task_),
		comlete(complete_)
	{

	}

	std::ostream & PersistenceString::open_stream()
	{
		return stream;
	}

	void PersistenceString::close_stream()
	{
		comlete(stream.str());
		stream.str(std::string());
		stream.clear();
	}
	
}}