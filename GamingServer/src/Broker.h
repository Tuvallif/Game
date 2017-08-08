/*
 * Broker.h
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#ifndef SRC_BROKER_H_
#define SRC_BROKER_H_
#include "TCPSocket.h"
#include "MThread.h"
#include "BrokerMng.h"
#include "MTCPListener.h"
#include "TCPMessangerProtocol.h"
#include <stdlib.h>

namespace npl {

class Broker:public MThread {


public:
	Broker(TCPSocket* peer1, TCPSocket* peer2, BrokerMng* parent);
	~Broker();
	virtual void run();
	int sendCommandToClient(TCPSocket * socket, int cmd,const char* buff);

private:
	bool  handleMessage(TCPSocket * socket, const char * commandStr);
	int readFromSocket(TCPSocket * socket,  char * buffer , int bytesToRead);
	TCPSocket* getOtherPeer(TCPSocket* socket);
	BrokerMng* parent;
	TCPSocket * peerOne;
	TCPSocket * peerTwo;
	MTCPListener* listener;

};

}/* namespace npl */

#endif /* SRC_BROKER_H_ */
