
#include "client.h"
#include "log.h"

#include <boost\asio.hpp>
#include <random>

namespace BTService { namespace Client {
	
	using boost::asio::ip::tcp;
	using io_service = boost::asio::io_service;

	struct Request
	{
		int client_id;
		int key;
	};

	struct Response
	{
		int avg;
	};


	Client::Client(int id_, const std::string & host_, const std::string& port_):
		id(id_), host(host_), port(port_)
	{
		LTRACE() << "constructor id=" << id << " host=" << host << " post=" << port;
	}

	Client::~Client()
	{
		LTRACE() << "destructor";
	}

	void Client::run()
	{
		LINFO() << "Starting...";

		std::random_device r;

		// Choose a random mean between 1 and 6
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> dis(0, 1023);

		io_service service;

		tcp::resolver resolver(service);
		tcp::resolver::query query(host, port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(service);
		boost::asio::connect(socket, endpoint_iterator);

		Request req;
		Response res;

		for (;;)
		{
			req.client_id = id;
			req.key = dis(e1);

			LTRACE() << "Send new request key=" << req.key;
			boost::asio::write(socket, boost::asio::buffer(static_cast<void*>(&req), sizeof(Request)));
			boost::asio::read(socket, boost::asio::buffer(static_cast<void*>(&res), sizeof(Response)));
			LTRACE() << "Response avg=" << res.avg;
		}

		LINFO() << "Finishing";
	}

}}