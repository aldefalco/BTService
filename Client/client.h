#pragma once

#include "log.h"

namespace BTService { namespace Client {

	class Client final
	{
	public:
		Client(int id, const std::string& host, const std::string& port);
		~Client();

		void run();

	private:

		Log::Logger logger{ Log::create("Client") };
		int id;
		std::string host;
		std::string port;

	};


}}