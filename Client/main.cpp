#include "stdafx.h"
#include "log.h"
#include "..\Server\data.h"
#include "client.h"

namespace BTService { namespace Client {

	constexpr char *VERSION = "0.0.0.1";

	int start(int argc, char* argv[])
	{
		int id;

		try 
		{
			if (argc < 4)
				throw;
			id = boost::lexical_cast<int>(argv[1]);
		}
		catch (const std::exception&)
		{
			std::cout << "Usage: client <client id> <host> <port>" << std::endl;
			return -1;
		}

		Log::setup(id);
		Log::Logger logger{ Log::create("Main") };

		try
		{
			LINFO() << "Client for binary tree service, version " << VERSION;
			Client client { id, argv[2], argv[3] };
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