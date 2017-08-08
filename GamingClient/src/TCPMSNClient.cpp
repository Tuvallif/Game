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

using namespace std;
using namespace npl;

bool sessionIsActive = false;

TCPMSNClient::TCPMSNClient() {
	socket = NULL;
}

TCPMSNClient::TCPMSNClient(TCPMessangerClientHandler* handler) {
	this->handler = handler;
	socket = NULL;

}

//TCPMessangerClient::TCPMessangerClient() {
//	this->handler = handler;
//	socket = NULL;
//}

TCPMSNClient::~TCPMSNClient() {
	if(socket != NULL){
		delete socket;
	}
	delete handler;
}

bool TCPMSNClient::connect(string ip) {
	socket = new TCPSocket(ip,MSNGR_PORT);
	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt(socket->socket(), SOL_SOCKET, SO_ERROR, &error, &len);
	if(retval != 0){
		cout << "there was an error opening the socket!" << endl;
		return false;
	}

	thread = new TCPMSNClientThread(socket);
	thread->start();
	return true;
}

bool TCPMSNClient::openSession(string ip, int port) {
	char buff[100];
	sprintf(buff,"%s:%d",ip.c_str(),port);
	cout<<"data to send:"<<buff<<endl;
	int res = sendCommand(OPEN_SESSION_WITH_PEER,buff);
	sessionIsActive = (res > -1);
	return sessionIsActive;
}

bool TCPMSNClient::loginRegister(string username, string password) {
	int res = sendCommand(LOGIN_REGISTER,username.c_str(), password.c_str());
	sessionIsActive = (res > -1);
	return sessionIsActive;
}

bool TCPMSNClient::getListOfUsers() {
	int res = sendCommand(GET_LIST_USERS,NULL);
	sessionIsActive = (res > -1);
	return sessionIsActive;
}

void TCPMSNClient::startMatchWithUser(string userName) {
	cout << "sending request to server to start match with user " + userName << endl;
	sendCommand(START_MATCH, userName.c_str());

}

void TCPMSNClient::seek() {
	cout << "sending request to server to start match with random user "<< endl;
	sendCommand(SEEK, NULL);

}

void TCPMSNClient::answerRequest(string answer){
	string userName = this->thread->getWAitingSocket();
	this->thread->setState(false);
	this->thread->setWAitingSocket("NULL");
	if(answer == "Y"|| answer == "y")
	{
		sendCommand(ACCEPT_REQUEST_MATCH, userName.c_str());
	}
	else
	{
		sendCommand(DECLINE_START_MATCH, userName.c_str());
	}
}

void TCPMSNClient::sendMessage(string msg) {
	cout<<"this message will be sent to peer "<< msg << endl << flush;//why flush?;
	if (!sessionIsActive)
	{
		sendCommand(SESSION_REFUSED, SESSION_REFUSED_MSG);
	}
	else{
		sendCommand(SEND_MSG_TO_PEER, msg.c_str());
	}
}

int TCPMSNClient::getPort(){
	return socket->getPort();
}

void TCPMSNClient::closeSession() {
	if (sessionIsActive){
		sendCommand(CLOSE_SESSION_WITH_PEER, " ");
		cout<<"Session was closed by other side"<<endl<<flush;
	}
	else{
		sendCommand(SESSION_REFUSED, SESSION_REFUSED_MSG);
	}
}

void TCPMSNClient::disconnect() {

	sendCommand(SESSION_REFUSED, "other user asked to disconnect");

	if (socket !=NULL){
		cout<<"disconnecting from socket"<<endl<<flush;
		closeSession();
		socket->close();
		//waitForThread();
		sendCommand(EXIT, " ");
		cout<<"communication with server was interrupted - connection closed"<< endl;
		cout<<"closing socket"<<endl<<flush;
	}
	else
		cout << "you're not connected"<<endl<<flush;
}

void TCPMSNClient::exit() {
	sendCommand(EXIT, "other user asked to exit");
}

int TCPMSNClient::sendCommand(int cmd,const char* buff, const char * buff2){
	int cmdNet = htonl(cmd);
	int res = socket->write((char*)&cmdNet,4);
	if (res < 4){
		return -1;
	}
	if (buff != NULL){
		int len = strlen(buff);
		int lenNet = htonl(len);
		res = socket->write((char*)&lenNet,4);
		if (res < 4){
			return -1;
		}
		res = socket->write(buff,len);
		if (res < len){
			return -1;
		}
	}
	if (buff2 != NULL){
		int len = strlen(buff2);
		int lenNet = htonl(len);
		res = socket->write((char*)&lenNet,4);
		if (res < 4){
			return -1;
		}
		res = socket->write(buff2,len);
		if (res < len){
			return -1;
		}
	}
	return res;
}


