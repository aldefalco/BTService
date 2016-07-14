#include "stdafx.h"
#include "log.h"
#include "..\Server\data.h"

#include "client.h"

namespace BTService { namespace Client {
	
	using boost::asio::ip::tcp;
	using io_service = boost::asio::io_service;

	Client::Client(int id_, const std::string & host_, const std::string& port_):
		id(id_), host(host_), port(port_)
	{
		LDEBUG() << "constructor id=" << id << " host=" << host << " post=" << port;
	}

	Client::~Client()
	{
		LDEBUG() << "destructor";
	}

	void Client::run()
	{
		LDEBUG() << "Starting...";

		io_service service;

		tcp::resolver resolver(service);
		tcp::resolver::query query(host, port);

		LTRACE() << "Resolving endpoint";

		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(service);
		
		LTRACE() << "Connecting...";
		boost::asio::connect(socket, endpoint_iterator);
		LTRACE() << "Connected";

		Request req{id, 0};
		Response res;

		for (;;)
		{
			req.key = generator.get_next();

			LTRACE() << "Send a new request key=" << req.key;
			boost::asio::write(socket, boost::asio::buffer(static_cast<void*>(&req), sizeof(Request)));
			LTRACE() << "Sending was success, read response";
			boost::asio::read(socket, boost::asio::buffer(static_cast<void*>(&res), sizeof(Response)));
			LTRACE() << "Response avg=" << res.avg;
		}

		LDEBUG() << "Finishing";
	}

	Client::Generator::Generator(int from, int to) :
		device(), engine(device()), dis(from, to)
	{
	}
	
	int Client::Generator::get_next()
	{
		return dis(engine);
	}
}}