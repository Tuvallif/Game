/*
 * Dispatcher.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#include "Dispatcher.h"
#include "MTCPListener.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include "TCPMessangerProtocol.h"

using namespace std;

namespace npl {
Dispatcher::Dispatcher(DispatcherHandler* handler) : MThread() {
	cout << "Dispatcher created" << endl;
	userDB = new UsersDataBase(USERS_DB_FILE);
	this->dispatcherHandler = handler;
	listener = new MTCPListener();
	start();
}

Dispatcher::~Dispatcher() {
	delete listener;
}

void Dispatcher::addPeer(TCPSocket* peer){
	cout << "Adding peer " << peer->fromAddr() << ":" << peer->peerPort();
	cout << endl;
	listener->add(peer);
};

void Dispatcher::seek(TCPSocket* requestingSocket){

}

int Dispatcher::readFromSocket(TCPSocket * socket,  char * buffer , int bytesToRead)
{
	int length = socket->recv(buffer, bytesToRead );
	if ( length != bytesToRead )
	{
		cout << "problem reading length " << length;
		cout << endl;
		// TOOD this is error close the socket
	}
	return length;
}

void Dispatcher::run() {

	const vector<TCPSocket *> & sockets = listener->socketsList();
	cout << "Dispatcher running" << endl;
	while (true )
	{
		if ( sockets.empty() )
		{
			cout << "Dispatcher waiting 1 second" << endl;
			usleep( 1000000 );
			continue;
		}
		//cout << "Dispatcher selecting " << sockets.size() << " sockets " << endl;
		TCPSocket * readySocket = listener->listen();
		if ( readySocket ) {
			cout << "Dispatcher socket ready" << endl;
			char commandType[4];
			int length = readFromSocket(readySocket, commandType, sizeof(commandType));

			if ( length == 0 )
			{
				cout << "socket error. disconnecting client" << endl;
				listener->remove(readySocket);
				continue;
			}
			int command = ntohl( *( (int * ) commandType ) );
			cout << "received command " << command << " from " << readySocket->fromAddr() << ":" << readySocket->peerPort();
			cout << endl;
			// TODO if command length is 0, close the peer and remove it from listener
			handleMessage(readySocket, command);
			// TODO : parse and handle the commands : disconnect, open session...
		}
	}
	cout << "Dispatcher terminated" << endl;
}
TCPSocket* Dispatcher::findSocketInMap(TCPSocket* socketKey){
	std::map<TCPSocket*, TCPSocket*>::iterator i = openRequests.find(socketKey);
	assert(i != openRequests.end());
	try{
		TCPSocket* sockToRtrn =  i->second;
		return sockToRtrn;
	}
	catch(exception e){
		cout << e.what() << endl;
		return NULL;
	}


}

int Dispatcher::sendCommandToClient(TCPSocket * socket, int cmd,const char* buff){
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
	return res;
}

void Dispatcher::handleMessage(TCPSocket * socket, int commandType )
{
	switch(commandType)
	{
	case OPEN_SESSION_WITH_PEER: {
		char messageLength[4];
		readFromSocket(socket, messageLength, sizeof(messageLength));
		int dataLength = ntohl( *( (int * ) ( messageLength ) ) );
		char dataBuffer[dataLength];
		readFromSocket(socket, dataBuffer, sizeof(dataBuffer));
		string data(dataBuffer, dataLength);
		cout << "got message length " << dataLength << " command " << data.c_str() << endl;
		const vector<TCPSocket *> & sockets = listener->socketsList();
		TCPSocket * foundPeer = NULL;
		for( vector<TCPSocket *>::const_iterator it = sockets.begin(); it != sockets.end() ; ++it)
		{
			TCPSocket * peer = * it;
			if ( socket == peer ) {
				// don't check against yourself
				continue;
			}
			char port[16];
			sprintf(port, "%d", peer->peerPort());
			string peerAddress = peer->fromAddr() + ":" + port;
			cout << "checking if requested peer " << data << " is " << peerAddress << endl;
			if ( data == peerAddress )
			{
				foundPeer = peer;
				break;
			}
		}
		if ( foundPeer == NULL )
		{
			// peer not found. send session refused to socket
			sendCommandToClient( socket, SESSION_REFUSED, NULL);
		} else{
			sendCommandToClient(socket, SESSION_ESTABLISHED, NULL);
			sendCommandToClient(foundPeer, SESSION_ESTABLISHED, NULL);
			listener->remove(socket);
			listener->remove(foundPeer);
			// create new broker and pass them to the broker
			dispatcherHandler->managePeerSession(socket, foundPeer);
		}
		break;
	}
	case SEEK:
	{
		userDB->seek(socket);
		//todo match them together
		break;
	}
	case START_MATCH:
	{
		char messageLength[4];
		readFromSocket(socket, messageLength, sizeof(messageLength));
		int dataLength = ntohl( *( (int * ) ( messageLength ) ) );
		char dataBuffer[dataLength];
		readFromSocket(socket, dataBuffer, sizeof(dataBuffer));
		string userName(dataBuffer, dataLength);
		int status = userDB->checkAvilability(userName);

		if(status == -1) {
			sendCommandToClient(socket, MATCH_REJECTED, "The match could not be established, user not found or playing");
		}
		else if(status == User::STATE_SEEKING)
		{
			//to do start match
		}
		else if(status == User::STATE_DEFAULT)
		{
			//for sure not null because requesting user
			char messageLength[4];
			readFromSocket(socket, messageLength, sizeof(messageLength));
			int dataLength = ntohl( *( (int * ) ( messageLength ) ) );
			char dataBuffer[dataLength];
			readFromSocket(socket, dataBuffer, sizeof(dataBuffer));
			string secondSocketName(dataBuffer, dataLength);
			TCPSocket * secondSocket = userDB->findUserByName(secondSocketName)->socket;
			string requestingSocketName = userDB->findUserBySocket(socket)->username;
			openRequests[secondSocket] = socket;

			sendCommandToClient(secondSocket, REQUEST_START_MATCH, requestingSocketName.c_str());
			//to do start match
		}
		else if(status == User::STATE_BUSY)
		{
			//for sure not null because requesting user
			char messageLength[4];
			readFromSocket(socket, messageLength, sizeof(messageLength));
			int dataLength = ntohl( *( (int * ) ( messageLength ) ) );
			char dataBuffer[dataLength];
			readFromSocket(socket, dataBuffer, sizeof(dataBuffer));
			string secondSocketName(dataBuffer, dataLength);
			TCPSocket * secondSocket = userDB->findUserByName(secondSocketName)->socket;
			string requestingSocketName = userDB->findUserBySocket(socket)->username;
			openRequests.insert(std::pair<TCPSocket*,TCPSocket*>(socket,secondSocket));
			sendCommandToClient(secondSocket, REQUEST_START_MATCH, requestingSocketName.c_str());
			sendCommandToClient(socket, DECLINE_START_MATCH, secondSocketName.c_str());
		}
		break;
	}


	case CLOSE_SESSION_WITH_PEER:
	{
		cout << "got message to close peer from " <<socket->getPort() << endl;
		cout << "can't close session because Broker handles this" << endl;
		break;
	}
	case EXIT:{
		sendCommandToClient(socket, EXIT, NULL);
		listener->remove(socket);
		break;
	}
	case SEND_MSG_TO_PEER:
	{
		cout << "got input to send message peer from " <<socket->getPort() << endl;
		cout << "can't send message because Broker handles this" << endl;
		break;
	}
	//when do i get those??
	case SESSION_REFUSED: break;
	case SESSION_ESTABLISHED: break;
	case LOGIN_REGISTER:
	{
		char usernameLengthBuff[4];
		readFromSocket(socket, usernameLengthBuff, sizeof(usernameLengthBuff));
		int usernameLength = ntohl( *( (int * ) ( usernameLengthBuff ) ) );
		char usernameBuffer[usernameLength];
		readFromSocket(socket, usernameBuffer, sizeof(usernameBuffer));
		string username(usernameBuffer, usernameLength);
		char passwordLengthBuff[4];
		readFromSocket(socket, passwordLengthBuff, sizeof(passwordLengthBuff));
		int passwordLength = ntohl( *( (int * ) ( passwordLengthBuff ) ) );
		char passwordBuffer[passwordLength];
		readFromSocket(socket, passwordBuffer, sizeof(passwordBuffer));
		string password(passwordBuffer, passwordLength);

		if(userDB->login(username, password, socket)){
			sendCommandToClient(socket, LOGIN_REGISTER_SUCCESFULL, NULL);
		}else{
			sendCommandToClient(socket, LOGIN_REGISTER_FAILED, NULL);
		}
		break;
	}
	case GET_LIST_USERS:
	{
		string onlineUsers = userDB->listUsersInString();
		sendCommandToClient(socket, GET_LIST_USERS, onlineUsers.c_str());
		break;
	}
	case ACCEPT_REQUEST_MATCH:
	{
		//TCP userName = userDB->findUserBySocket(socket)->username;

		TCPSocket* otherSocket = findSocketInMap(socket);
		if (otherSocket == NULL)
		{
			// peer not found. send session refused to socket
			sendCommandToClient(socket, SESSION_REFUSED, NULL);
		} else{
			string firstSocketName = userDB->findUserBySocket(socket)->username;
			string seconfSocketName = userDB->findUserBySocket(otherSocket)->username;
			sendCommandToClient(socket, GAME_STARTED, seconfSocketName.c_str());
			sendCommandToClient(otherSocket, GAME_STARTED, firstSocketName.c_str());
			listener->remove(socket);
			listener->remove(otherSocket);
			// create new broker and pass them to the broker
			dispatcherHandler->managePeerSession(socket, otherSocket);
		}
		break;
	}
	default:
	{
		sendCommandToClient(socket, SEND_MSG_TO_PEER, "Your request was not identified, Please try again");
		break;
	}
	}
	//int length = *( (int * ) ( commandStr.c_str() + 4 ) );
	//char * data = commandStr.c_str() + 8;
}


} /* namespace npl */
