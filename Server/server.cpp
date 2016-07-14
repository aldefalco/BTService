#include "stdafx.h"
#include "persistence.h"
#include "storage.h"
#include "log.h"

#include "server.h"

using boost::asio::ip::tcp;

namespace BTService { namespace Server {

	Server::Server(int port_, int timeout) :  port(port_)
	{
		LDEBUG() << "Initial server instance timeout=" << timeout << "port=" << port;
		
		storage = std::shared_ptr<Storage>(new Storage());
		
		persistence = std::shared_ptr<Persistence>(new PersistenceFile("bt.bin", timeout, 
			[=](std::ostream& s) {
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

		persistence->start();

		boost::asio::io_service service;

		tcp::acceptor a(service, tcp::endpoint(tcp::v4(), port));
		for (;;)
		{
			SoketPointer socket(new tcp::socket(service));
			a.accept(*socket);
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

				socket->read_some(boost::asio::buffer(static_cast<void*>(&req), sizeof(Request)), error);
				
				if (error == boost::asio::error::eof)
				{
					LDEBUG() << "Connection closed";
					break;
				}
				else if (error)
				{
					throw boost::system::system_error(error); 
				}

				LDEBUG() << "New request incomming id=" << req.client_id << " key=" << req.key;

				res.avg = storage->update(req.key);

				boost::asio::write(*socket, boost::asio::buffer(static_cast<void*>(&res), sizeof(Response)));
				
				LDEBUG() << "Response was sent avg=" << res.avg;

			}
		}
		catch (std::exception& e)
		{
			LERROR() << "Internal server exception" << e.what();
		}
	}

}}