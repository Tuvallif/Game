/*
 * TCPMSNClient.cpp
 *
 *  Created on: May 29, 2017
 *      Author: user
 */

#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "TCPMessangerProtocol.h"
#include "MThread.h"
#include "TCPMSNClient.h"
#include "MTCPListener.h"

using namespace std;
using namespace npl;

TCPMSNClientThread::TCPMSNClientThread(TCPSocket * socket) :
		MThread()
{
	this->socket = socket;
	string waitingSocketName = "NULL";
	bool currStateWaiting = false;
	listener = new MTCPListener();
}

//move later

vector<string> splitString(const string& inputString, const char* delim){
	vector<string> result;
	string copy = inputString;
	char * token = strtok((char *)copy.c_str(), delim);
	while(token != NULL){
		result.push_back(token);
		token= strtok(NULL, delim);
	}

	return result;
}

bool TCPMSNClientThread::getState(){
	return currStateWaiting;
}
string TCPMSNClientThread::getWAitingSocket(){
	return waitingSocketName;
}
void TCPMSNClientThread::setState(bool newState){
	this->currStateWaiting = newState;
}
void TCPMSNClientThread::setWAitingSocket(string newName){
	this->waitingSocketName = newName;
}

void TCPMSNClientThread::run()
{
	cout << "client thread running" << endl;

	// TODO add stop boolean to stop when session disconnect
	while (true)
	{
		cout << "client thread listening" << endl;
		listener->add(socket);
		listener->listen();

		char buffer[1024];
		int len = socket->recv(buffer, 1024);

		int msg = htonl(* ( (int *) buffer ) );
		cout << "got message " << msg << " length " << len << endl;

		if ( msg == SEND_MSG_TO_PEER)
		{
			int msgLength = htonl(* ( (int *) &buffer[4] ) );
			string msgText(&buffer[8],msgLength);
			cout << "received message : " << msgText << endl;

		}
		else if ( msg == GET_LIST_USERS)
		{
			int msgLength = htonl(* ( (int *) &buffer[4] ) );
			string msgText(&buffer[8],msgLength);
			cout << "online users : " << msgText << endl;
			vector<string> onlineUsers = splitString(msgText, USER_NAME_SEPERATOR);
			for(int index = 0;index<onlineUsers.size();index++) {
				cout << index << " : " << onlineUsers[ index ] << endl;
			}

		}
		else if ( msg == LOGIN_REGISTER_SUCCESFULL)
		{
			cout << "You are is now logged in" << endl;

		}
		else if ( msg == LOGIN_REGISTER_FAILED)
		{
			cout << "login failed, please try again" << endl;
		}
		else if(msg == MATCH_REJECTED){
			int msgLength = htonl(* ( (int *) &buffer[4] ) );
			string msgText(&buffer[8],msgLength);
			cout << msgText << endl;
		}
		else if(msg == REQUEST_START_MATCH){
			int msgLength = htonl(* ( (int *) &buffer[4] ) );
			string userName(&buffer[8],msgLength);
			if(currStateWaiting == true){
				cout << "You did not reply to " << waitingSocketName << "'s request - please reply." << endl;
				cout << "You got a new request from " << userName << " but it was ignored" << endl;
			}
			else{
			waitingSocketName = userName;
			cout << "You got a new match request from " << userName << endl;
			cout << "would you like to accept the new match with " << userName << "? format: r  <yes/no>" << endl;
			}

		}

	}




	cout << "client thread stopped" << endl;
}
