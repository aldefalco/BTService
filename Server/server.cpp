#include "stdafx.h"
#include "log.h"
#include "data.h"
#include "persistence.h"
#include "storage.h"
#include "server.h"

using boost::asio::ip::tcp;


namespace BTService { namespace Server {

	Server::Server(int port_, int timeout) :  port(port_)
	{
		LDEBUG() << "Initial server instance timeout=" << timeout << " port=" << port;
		
		storage = std::shared_ptr<Storage>(new Storage());
		
		persistence = std::shared_ptr<Persistence>(new PersistenceFile(PERSISTENCE_FILENAME, timeout,
			[=](std::ostream& s) {
			LTRACE() << "Persistance task call";
			storage->serialize(s);
		}));
	}

	Server::~Server()
	{
		LDEBUG() << "Destroy server instance";
		persistence->stop();
	}

	void Server::run()
	{
		LDEBUG() << "Run server";
		
		LTRACE() << "Persistence starting...";
		persistence->start();
		LTRACE() << "Persistence started";

		boost::asio::io_service service;

		tcp::acceptor a(service, tcp::endpoint(tcp::v4(), port));

		for (;;)
		{
			LTRACE() << "Accepting incoming connections...";
			SoketPointer socket(new tcp::socket(service));
			a.accept(*socket);
			LTRACE() << "Connected";
			std::thread t(&Server::_session, this, socket);
			t.detach();
		}
	}

	void Server::_session(SoketPointer socket)
	{
		LDEBUG() << "New session is comming";
		try
		{
			for (;;)
			{
				Request req;
				Response res;

				boost::system::error_code error;

				LTRACE() << "Wait for next request...";
				int ret = socket->read_some(boost::asio::buffer(static_cast<void*>(&req), sizeof(Request)), error);

				LTRACE() << "Read a new request. size=" << ret;
				
				if (error == boost::asio::error::eof)
				{
					LWARNING() << "Connection closed";
					break;
				}
				else if (error)
				{
					LERROR() << "Network error:" << error;
					throw boost::system::system_error(error); 
				}

				LTRACE() << "New request incomming id=" << req.client_id << " key=" << req.key;

				res.avg = storage->update(req.key);

				LTRACE() << "Sending...";

				boost::asio::write(*socket, boost::asio::buffer(static_cast<void*>(&res), sizeof(Response)));
				
				LTRACE() << "Response was sent avg=" << res.avg;
			}
		}
		catch (std::exception& e)
		{
			LERROR() << "Internal server exception" << e.what();
		}
	}

}}