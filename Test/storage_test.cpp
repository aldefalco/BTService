#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Server\log.h"
#include "..\Server\storage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{	
	using BTService::Server::Storage;
	using StoragePointer = std::unique_ptr<Storage>;
	

	TEST_CLASS(StorageTest)
	{

		TEST_METHOD_INITIALIZE(setup) 
		{
			storage = StoragePointer(new Storage());
		}

	public:
		
		TEST_METHOD(should_update_one_number)
		{
			const int key = 23;

			auto result =  storage->update(key);
			
			Assert::AreEqual(key*key, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_update_two_number)
		{
			const int keys[] { 23, 1023 };

			int result{ -1 };
			int aggregate{ 0 };

			for (auto key : keys)
			{
				result = storage->update(key);
				aggregate += key*key;
			}

			Assert::AreEqual(aggregate/2, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_ignore_negative_numbers)
		{
			const int keys[]{ 23, -2 };
			const int expected = 23 * 23;

			int result{ -1000 };

			for (auto key : keys)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_ignore_duplication_numbers)
		{
			const int keys[]{ 23, 23 };
			const int expected = 23 * 23;

			int result{ -1000 };

			for (auto key : keys)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_ignore_more_then_1024)
		{
			const int keys[]{ 23, 2000 };
			const int expected = 23 * 23;

			int result{ -1000 };

			for (auto key : keys)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_handle_0_number)
		{
			const int keys[]{ 23, 0 };
			const int expected = 23 * 23 /2;

			int result{ -1000 };

			for (auto key : keys)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_handle_only_0_number)
		{
			const int keys[]{ 0 };
			const int expected = 0;

			int result{ -1000 };

			for (auto key : keys)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}


		TEST_METHOD(should_handle_only_negotive_number)
		{
			const int keys[]{ -1 };
			const int expected = 0;

			int result{ -1000 };

			for (auto key : keys)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}

		TEST_METHOD(should_handle_1024_numbers)
		{
			const int expected = 1023*(2*1023+1)/6;

			int result{ -1000 };

			for (int key = 0; key < 1024; key++)
			{
				result = storage->update(key);
			}

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}


		TEST_METHOD(should_serialize_to_stream)
		{
			// TODO: this test seems to be failed now. It needs to be rewritten as the expectation should be a binary structure
			const std::string expected = "BT\n285\n10\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n";

			std::stringstream s;
			
			for (int key = 0; key < 10; key++)
			{
				storage->update(key);
			}

			storage->serialize(s);

			const std::string result = s.str();

			Assert::AreEqual(expected, result, L"invalid update", LINE_INFO());
		}


	private:
		StoragePointer storage;
	};
}