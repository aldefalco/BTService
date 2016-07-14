#pragma once

// Server transport 

using boost::asio::ip::tcp;

namespace BTService { namespace Server {

	struct Request
	{
		int client_id;
		int key;
	};

	struct Response
	{
		int avg;
	};


	class Server
	{
	public:
		
		using SoketPointer = boost::shared_ptr<tcp::socket>;

		Server(int port, int timeout);
		~Server();

		void run();
		

	private:
		void _session(SoketPointer socket);

	private:
		int port;
		
		std::shared_ptr<Persistence> persistence;
		std::shared_ptr<Storage> storage;
		Log::Logger logger{ Log::create("Server") };
	};
	
}}