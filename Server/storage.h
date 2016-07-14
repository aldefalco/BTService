#pragma once

// Binary tree storage for service

namespace BTService { namespace Server {

	// This is a lazy and simple thread-safe implementation for defined requirements
	class Storage
	{
	public:
		Storage();
		~Storage();

		int update(int key);
		void serialize(std::ostream& s);

	private:
	
		std::set<int> tree{};
		int square_sum_cache{ 0 };
		std::mutex mutex;
	};

}}
