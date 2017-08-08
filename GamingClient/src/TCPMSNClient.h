/*
 * TCPMSNClient.h
 *
 *  Created on: May 29, 2017
 *      Author: user
 */

#ifndef SRC_TCPMSNCLIENT_H_
#define SRC_TCPMSNCLIENT_H_

#include <iostream>
#include "TCPSocket.h"
#include <sstream>
#include <string>
#include <string.h>
#include <pthread.h>
#include "MThread.h"
#include "TCPMessangerProtocol.h"
#include "TCPMSNClientThread.h"

using namespace std;
namespace npl {

class TCPMessangerClientHandler{
public:
	virtual void onMessageRecieved(string message)=0;
	virtual void onOpenSession(string ip, int port)=0;
	virtual void onClosedSession()=0;
	virtual ~TCPMessangerClientHandler(){}

};

class TCPMSNClient {
private:
	TCPMessangerClientHandler *handler;
	TCPSocket *socket;
	TCPMSNClientThread * thread;
	int sendCommand(int cmd,const char* buff, const char * buff2 = NULL);

public:

	TCPMSNClient();
	TCPMSNClient(TCPMessangerClientHandler* handler);

	//	TCPMessangerClient();

	virtual ~TCPMSNClient();

	/**
	 * connects the client to the remote server on the given ip,
	 * the server port is read from the protocol file
	 *
	 * @param ip - the ip of the server to connect to
	 *
	 * @return true on success otherwise false
	 */
	bool connect(string ip);

	bool openSession(string ip, int port);

	bool loginRegister(string username, string password);

	bool getListOfUsers();

	void startMatchWithUser(string userName);

	void seek();

	void answerRequest(string answer);

	void sendMessage(string msg);

	void closeSession();

	void disconnect();

	void exit();

	int getPort();

};


} /* namespace npl */

#endif /* SRC_TCPMSNCLIENT_H_ */
