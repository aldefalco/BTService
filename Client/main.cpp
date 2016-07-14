
#include "log.h"
#include "client.h"

#include <boost\lexical_cast.hpp>

namespace BTService { namespace Client {

	constexpr char *VERSION = "0.0.0.1";

	int start(int argc, char* argv[])
	{
		Log::setup();
		Log::Logger logger{Log::create("Main")};

		LINFO() << "Client for binary tree service, version " << VERSION ;

		if (argc < 4)
		{
			LERROR() << "Usage: client <client id> <host> <port>";
			return -1;
		}

		try
		{
			Client client { boost::lexical_cast<int>(argv[1]), argv[2], argv[3] };
			client.run();
		}
		catch (const std::exception& e)
		{
			LERROR() << "Global exception:" << e.what();
			return -1;
		}

		return 0;
	}
}}

int main(int argc, char* argv[])
{
	return BTService::Client::start(argc, argv);
}