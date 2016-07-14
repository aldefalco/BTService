#pragma once

#include "log.h"

namespace BTService { namespace Client {

	// Client for the BT service
	class Client final
	{
	public:
		/*!
		* Constructor
		* \param id The Client id
		* \param host The BT Service's host name or ip address
		* \param port The BT Service's port or IP service name
		*/
		Client(int id, const std::string& host, const std::string& port);
		~Client();

		/*! Run the client in infinity loop
		*/
		void run();

	private:
		
		// Random numbers generator
		class Generator
		{
		public:
			/*! 
			* Construct random generator of integer numbers
			* \param from The first number of the sequence
			* \param to The last number of the sequence
			*/
			Generator(int from, int to);

			/*!
			* Get next random number
			* \return The next number
			*/
			int get_next();

		private:
			std::random_device device;
			std::default_random_engine engine;
			std::uniform_int_distribution<int> dis;
		};

	private:
		Log::Logger logger{ Log::create("Client") };
		int id;
		std::string host;
		std::string port;
		Generator generator{ KEY_FROM, KEY_TO };

	};


}}