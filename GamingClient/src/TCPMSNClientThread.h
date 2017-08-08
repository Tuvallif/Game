/*
 * TCPMSNClient.h
 *
 *  Created on: May 29, 2017
 *      Author: user
 */

#ifndef SRC_TCPMSNCLIENT_THREAD_H_
#define SRC_TCPMSNCLIENT_THREAD_H_

#include <iostream>
#include "TCPSocket.h"
#include <sstream>
#include <string>
#include <string.h>
#include <pthread.h>
#include "MThread.h"
#include "TCPMessangerProtocol.h"
#include "TCPMSNClient.h"
#include "MTCPListener.h"

using namespace std;
namespace npl {


class TCPMSNClientThread : public MThread {
public:
	TCPMSNClientThread(TCPSocket * socket);
	bool getState();
	string getWAitingSocket();
	void setState(bool newState);
	void setWAitingSocket(string newName);
	virtual void run();
private:
	string waitingSocketName;
	bool currStateWaiting;
	void removeUnansweredRequst(string newSocketName);
	TCPSocket * socket;
	MTCPListener * listener;
};


} /* namespace npl */

#endif /* SRC_TCPMSNCLIENT_THREAD_H_ */
