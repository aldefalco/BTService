#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Server\log.h"
#include "..\Server\persistence.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::chrono_literals;

namespace Test
{
	using BTService::Server::PersistenceString;

	TEST_CLASS(PersistenceTest)
	{

	public:

		TEST_METHOD(simple_serialization)
		{
			std::mutex result_mutex;
			std::condition_variable complete_contition;
			

			std::string expected = "Test String";
			std::string result;
			PersistenceString instance(
				0, 
				[=](std::ostream& s) -> void {
					s << expected;
				},
				[&](const std::string& str) -> void {
					result = str;
					std::unique_lock<std::mutex> started_lock(result_mutex);
					complete_contition.notify_one();
				}
				);

			instance.start();
			//std::this_thread::sleep_for(1s);
			{
				std::unique_lock<std::mutex> lock(result_mutex);
				complete_contition.wait(lock);
			}
			
			instance.stop();

			Assert::AreEqual(expected, result, L"invalid", LINE_INFO());
		}
	};
}