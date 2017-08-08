/*
 * BrokerMng.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#include "BrokerMng.h"
#include <algorithm>
#include <vector>
#include "Broker.h"
#include <iterator>

using namespace std;

namespace npl {

BrokerMng::BrokerMng(BrokerMngHandler* handler){
	this->handler = handler;
	brokers = new vector<Broker*>();
}

BrokerMng::~BrokerMng()
{
	delete brokers;
}

void BrokerMng::createBroker(TCPSocket* peer1, TCPSocket* peer2){
	Broker* broker = new Broker(peer1, peer2, this);
	brokers->push_back(broker);
	broker->run();
};

void BrokerMng::rleasePeer(TCPSocket* peer){
	handler->handleReturnedPeer(peer);
}
void BrokerMng::deleteBroker(Broker* broker){

	//deletes the broker element

	brokers->erase(std::remove(brokers->begin(), brokers->end(), broker), brokers->end());
	delete broker;
}







} /* namespace npl */
