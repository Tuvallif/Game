/*
 * Dispatcher.h
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include "TCPSocket.h"
#include "MThread.h"
#include "TCPMessangerProtocol.h"
#include "MTCPListener.h"
#include "BrokerMng.h"
#include "UsersDataBase.h"
#include <vector>
#include <iostream>
#include <map>
#include <assert.h>
#include <iterator>

namespace npl {

class DispatcherHandler {
public:
	virtual void managePeerSession(TCPSocket* peer1, TCPSocket* peer2)=0;
	virtual ~DispatcherHandler(){}
};

class Dispatcher : public MThread {
	DispatcherHandler* dispatcherHandler;

public:
	Dispatcher(DispatcherHandler* handler);
	~Dispatcher();
	void addPeer(TCPSocket* peer);
	void close(){};
	void run();
private:
	UsersDataBase* userDB;
	MTCPListener * listener;
	map <TCPSocket*, TCPSocket*> openRequests;
	int readFromSocket(TCPSocket * socket,  char * buffer , int bytesToRead);
	void handleMessage(TCPSocket * socket, int commandType );
	int sendCommandToClient(TCPSocket * socket, int cmd,const char* buff);
	void seek(TCPSocket* requestingSocket);
	TCPSocket* findSocketInMap(TCPSocket* socketKey);

};

} /* namespace npl */

#endif /* SRC_DISPATCHER_H_ */
