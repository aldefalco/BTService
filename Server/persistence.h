#pragma once

namespace BTService { namespace Server {

	// Persistence background task
	class Persistence
	{
	public:
		using Task = std::function<void(std::ostream&)>; // void (*)(std::ostream& s);
		//using StreamFactory = std::ostream&& (*)(const std::string &name);

		Persistence(int timeout, Task task);
		//Persistence(std::ostream && stream, int timeout, Task task);
		virtual ~Persistence();

		void start();
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
		using Complete = std::function<void(const std::string&)>;//void (*)(const std::string& str);

		PersistenceString(int timeout, Task task, Complete complete);

	private:
		std::ostream & open_stream() override;
		void close_stream() override;

	private:
		
		std::stringstream stream{};
		Complete comlete;

	};

}}