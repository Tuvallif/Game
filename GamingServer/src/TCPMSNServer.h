/*
 * TCPMSNServer.h
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#ifndef SRC_TCPMSNSERVER_H_
#define SRC_TCPMSNSERVER_H_
#include <src/Server.h>
#include "Dispatcher.h"
#include "BrokerMng.h"


namespace npl {

class TCPMSNServer: public ServerHandler,DispatcherHandler,BrokerMngHandler {
private:
	Server* server;
	Dispatcher* dispatcher;
	BrokerMng* brokerMng;



public:
	TCPMSNServer();
	virtual ~TCPMSNServer();
	virtual void handlePeer(TCPSocket* peer);
	virtual void managePeerSession(TCPSocket* peer1, TCPSocket* peer2);
	virtual void handleReturnedPeer(TCPSocket* peer);

};

} /* namespace npl */

#endif /* SRC_TCPMSNSERVER_H_ */
