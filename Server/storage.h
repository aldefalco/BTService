#pragma once

// Binary tree storage for service

namespace BTService { namespace Server {

	// This is a lazy and simple thread-safe implementation for defined requirements
	class Storage
	{
	public:
		/*!
		* Construcor
		*/
		Storage();
		~Storage();

		/*!
		* Update storage by a new number
		* \param key The new number key for storage in the range of 0-1023
		* \return The average of squares
		*/
		int update(int key);
		
		/*!
		* Serialize storage (binary tree) to stream
		* \param stream The output stream
		*/
		void serialize(std::ostream& s);

	private:
		std::set<int> tree{};
		int square_sum_cache{ 0 };
		std::mutex mutex;

		Log::Logger logger{ Log::create("Storage") };
		
	};

}}
