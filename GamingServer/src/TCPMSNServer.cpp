/*
 * TCPMSNServer.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#include <src/Server.h>
#include "TCPMSNServer.h"
#include "Dispatcher.h"
#include "TCPMessangerProtocol.h"

namespace npl {

TCPMSNServer::TCPMSNServer(){
	cout << "Creating TCPMSNServer" << endl;

	server = new Server(this);
	dispatcher = new Dispatcher(this);
	brokerMng = new BrokerMng(this);
	server->run();
}

TCPMSNServer::~TCPMSNServer(){

}
void TCPMSNServer::handlePeer(TCPSocket* peer){
	this->dispatcher->addPeer(peer);
}
void TCPMSNServer::managePeerSession(TCPSocket* peer1, TCPSocket* peer2){
	this->brokerMng->createBroker(peer1,peer2);
}
void TCPMSNServer::handleReturnedPeer(TCPSocket* peer){
	this->dispatcher->addPeer(peer);
}



} /* namespace npl */

int main(){
	cout << "Creating server " << endl;
	npl::TCPMSNServer * server = new npl::TCPMSNServer();
	return 0;
}

