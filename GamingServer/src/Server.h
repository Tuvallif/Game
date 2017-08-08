/*
 * Server.h
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_
#include "TCPSocket.h"

namespace npl {



class ServerHandler{
public:
	virtual void handlePeer(TCPSocket* peer)=0;
	virtual ~ServerHandler();
};

class Server {
	ServerHandler* handler;
public:
	Server(ServerHandler* handler);
	~Server();
	void run();
};

} /* namespace npl */

#endif /* SRC_SERVER_H_ */
