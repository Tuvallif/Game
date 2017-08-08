/*
 * Server.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#include <src/Server.h>
#include <src/Server.h>
#include "TCPMessangerProtocol.h"

using namespace std;

namespace npl {

	Server::Server(ServerHandler* handler){
		this->handler = handler;
	}

	void Server::run()
	{
		cout << "Creating server socket on port " << MSNGR_PORT << endl;
		TCPSocket serverSocket( MSNGR_PORT );
		while ( true )
		{
			TCPSocket * clientSocket = serverSocket.listenAndAccept();
			cout << "New connection from " << clientSocket->fromAddr() << ":" <<
				clientSocket->peerPort() << endl;
			this->handler->handlePeer(clientSocket);
		}
	}
	Server::~Server()
	{
		delete this->handler;
		cout << "Server terminated" << endl;
	}

	ServerHandler::~ServerHandler()
	{
		cout << "Server handler terminated" << endl;
	}

} /* namespace npl */
