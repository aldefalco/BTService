#include "stdafx.h"
#include "log.h"
#include "data.h"
#include "storage.h"

namespace BTService { namespace Server {

	Storage::Storage()
	{
		LDEBUG() << "Storage instance creating";
	}

	Storage::~Storage()
	{
		LDEBUG() << "Storage instance destroying";
	}

	int Storage::update(int key)
	{
		LDEBUG() << "Storage update with key=" << key;

		std::lock_guard<std::mutex> lock(mutex);

		if (KEY_FROM <= key && key <= KEY_TO)
		{
			LTRACE() << "Trying to insert";
			auto inseted = tree.insert(key);

			// if new key inseted, update cache
			if (inseted.second)
			{
				square_sum_cache += key*key;
				LTRACE() << "Square sum updated:" << square_sum_cache;
			}
		}
		else 
		{
			LWARNING() << "Key is out of range";
		}

		// Divide by zero
		int size = tree.size();
		if (size == 0)
		{
			LWARNING() << "Size is 0";
			return 0;
		}

		int result = square_sum_cache / size;
		LTRACE() << "Update result" << result;

		return result;
	}

	void Storage::serialize(std::ostream & s)
	{
		LDEBUG() << "Storage serialization";
		std::lock_guard<std::mutex> lock(mutex);

		// Note: we use separator \n just for the test reasons. 
		// Write a header
		const std::string header = "BT";
		int size = tree.size();
		
		LTRACE() << "Write header";
		s.write(header.c_str(), header.size());
		
		// Write a sum and size
		//s << square_sum_cache << "\n";
		LTRACE() << "Write sum and size";
		s.write(reinterpret_cast<char*>(&square_sum_cache), sizeof square_sum_cache);
		s.write(reinterpret_cast<char*>(&size), sizeof square_sum_cache);

		//s << tree.size() << "\n";

		LTRACE() << "Write every items";
		for (auto i : tree)
		{
			s.write(reinterpret_cast<char*>(&i), sizeof i);
			//s << i << "\n";
		}
		LTRACE() << "Serialization is complete";
	}

}}
