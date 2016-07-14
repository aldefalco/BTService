#pragma once

// Server transport 

using boost::asio::ip::tcp;

namespace BTService { namespace Server {

	/*!
	* Server implementation
	*/
	class Server
	{
	public:
		using SoketPointer = boost::shared_ptr<tcp::socket>;

		/*
		* Constructor
		* \param port The port number to listen for
		* \param timeout The timeout for persistence
		*/
		Server(int port, int timeout);
		~Server();

		/*
		* Run the server instance in infinity loop
		*/
		void run();

	private:
		// New session handler
		void _session(SoketPointer socket);

	private:
		int port;
		std::shared_ptr<Persistence> persistence; // TODO: they might be the unique_ptrs for some reasons
		std::shared_ptr<Storage> storage;
		Log::Logger logger{ Log::create("Server") };
	};
	
}}