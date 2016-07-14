#include "stdafx.h"
#include "log.h"

#include "storage.h"
#include "persistence.h"
#include "server.h"

namespace BTService { namespace Server {

	constexpr char *VERSION = "0.0.0.1";

	int run(int argc, char* argv[])
	{
		Log::setup();
		Log::Logger logger{Log::create("Main")};

		LINFO() << "Binary tree service, version " << VERSION ;

		if (argc < 2)
		{
			LERROR() << "Usage: server <port> <serialization timeout in seconds>";
			return -1;
		}

		try
		{
			Server server{ boost::lexical_cast<int>(argv[1]), boost::lexical_cast<int>(argv[2]) };
			server.run();
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
	return BTService::Server::run(argc, argv);
}