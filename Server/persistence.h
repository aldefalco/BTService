#pragma once

namespace BTService { namespace Server {

	// Persistence background task
	class Persistence
	{
	public:
		using Task = std::function<void(std::ostream&)>; 

		/*!
		* Constructor
		* \param timeout The timeout between background task execution
		* \param task The task functor, it takes a ostream as parametr for serialization and called in background thread
		*/
		Persistence(int timeout, Task task);
		virtual ~Persistence();

		/*!
		* Start persistence background thread
		*/
		void start();
		/*
		* Stop background thread
		*/
		void stop();

	protected:
		
		// Gets a stream reference
		// Should be implement in derived classes
		virtual std::ostream & open_stream() = 0;

		// close stream
		virtual void close_stream();

	private:

		// Template method implementation that starts in a separate background thread
		void _task_template();

	protected:
		Log::Logger logger{ Log::create("Persistence") };

		

	private:
		std::thread thread;
		std::timed_mutex stopped;
		std::mutex started;
		std::condition_variable started_contition;
		int timeout;
		Task task;
	};


	// Persist to binary file
	class PersistenceFile : public Persistence
	{
	public:

		/*!
		* Constructor 
		* \param file The file name for binary persistence
		* \param timeout The timeout between background task execution
		* \param task The task functor, it takes a ostream as parametr for serialization and called in background thread
		*/
		PersistenceFile(const std::string& file, int timeout, Task task);

	private:
		std::ostream & open_stream() override;
		void close_stream() override;

	private:
		std::string file;
		std::ofstream stream;
	};
	

	// Persist to string for test purpouses
	class PersistenceString : public Persistence
	{
	public:
		using Complete = std::function<void(const std::string&)>;

		/*!
		* Constructor
		* \param complete The functor that will be called when the persisting routine completed. It taks a result string parametr.
		* \param timeout The timeout between background task execution
		* \param task The task functor, it takes a ostream as parametr for serialization and called in background thread
		*/
		PersistenceString(int timeout, Task task, Complete complete);

	private:
		std::ostream & open_stream() override;
		void close_stream() override;

	private:
		
		std::stringstream stream{};
		Complete comlete;

	};

}}