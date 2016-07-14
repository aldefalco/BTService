#include "stdafx.h"
#include "persistence.h"

namespace BTService { namespace Server {

	Persistence::Persistence(int timeout_, Task task_):
		timeout(timeout_),
		task(task_)
	{
	}

	/*
	Persistence::Persistence(const std::string& file_, int timeout_, Task task_):
		file(file_),
		timeout(timeout_),
		task(task_),
		stream_factory(stream_factory_)
	{
	}
	*/

	Persistence::~Persistence()
	{
		close_stream();
	}

	void Persistence::start()
	{
		stopped.lock();
		//std::unique_lock<std::mutex> started_lock(started);
		thread = std::thread(&Persistence::_task_template, this);
		//started_contition.wait(started_lock);
	}

	void Persistence::stop()
	{
		stopped.unlock();
		thread.join();
	}

	void Persistence::close_stream()
	{
	}

	void Persistence::_task_template()
	{
		//std::unique_lock<std::mutex> started_lock(started);
		using sec = std::chrono::seconds;
		while (!stopped.try_lock_for(sec(timeout)))
		{
			//started_contition.notify_one();
			std::ostream & s = open_stream();
			task(s);
			close_stream();
		} 
	}

	PersistenceFile::PersistenceFile(const std::string & file_, int timeout_, Task task_):
		Persistence(timeout_, task_),
		file(file_)
	{
		stream = std::ofstream(file, std::ios_base::binary);
	}

	std::ostream & Server::PersistenceFile::open_stream()
	{
		stream.open(file, std::ios_base::binary);
		return stream;
	}

	void PersistenceFile::close_stream()
	{
		stream.close();
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