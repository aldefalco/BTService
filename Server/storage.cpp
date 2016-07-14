#include "stdafx.h"
#include "storage.h"
#include <fstream> 

namespace BTService { namespace Server {

	Storage::Storage()
	{
	}

	Storage::~Storage()
	{
	}

	int Storage::update(int key)
	{
		std::lock_guard<std::mutex> lock(mutex);

		if (0 <= key && key < 1024)
		{
			auto inseted = tree.insert(key);

			// if new key inseted, update cache
			if (inseted.second)
			{
				square_sum_cache += key*key;
			}
		}

		return square_sum_cache/tree.size();
	}

	void Storage::serialize(std::ostream & s)
	{
		std::lock_guard<std::mutex> lock(mutex);

		// Note: we use separator \n just for the test reasons. 
		// Write a header
		const std::string header = "BT";
		int size = tree.size();
		
		s.write(header.c_str(), header.size());

		// Write a sum and size
		//s << square_sum_cache << "\n";
		s.write(reinterpret_cast<char*>(&square_sum_cache), sizeof square_sum_cache);
		s.write(reinterpret_cast<char*>(&size), sizeof square_sum_cache);

		//s << tree.size() << "\n";

		for (auto i : tree)
		{
			s.write(reinterpret_cast<char*>(&i), sizeof i);
			//s << i << "\n";
		}
	}

}}
