/*
 * BrokerMng.h
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#ifndef SRC_BROKERMNG_H_
#define SRC_BROKERMNG_H_

#include "TCPSocket.h"
#include <vector>
#include <algorithm>

namespace npl {

// forward declaration since the header file does not use methods
// of broker
class Broker;

class BrokerMngHandler{
public:
	virtual void handleReturnedPeer(TCPSocket* peer)=0;
	virtual ~BrokerMngHandler(){}
};


class BrokerMng {
private:
	BrokerMngHandler* handler;
	vector<Broker *> * brokers;

public:
	BrokerMng(BrokerMngHandler* handler);
	~BrokerMng();
	void createBroker(TCPSocket* peer1, TCPSocket* peer2);

	void rleasePeer(TCPSocket* peer);
	void deleteBroker(int i);
	void deleteBroker(Broker* broker);

};

} /* namespace npl */

#endif /* SRC_BROKERMNG_H_ */

