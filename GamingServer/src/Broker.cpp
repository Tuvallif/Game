/*
 * Broker.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: user
 */

#include "Broker.h"
#include "BrokerMng.h"
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <netinet/in.h>
#include <sys/socket.h>

using namespace std;

namespace npl {


char* getCharFromString(string toConvert){
	char * writable = new char[toConvert.size() + 1];
	std::copy(toConvert.begin(), toConvert.end(), writable);
	writable[toConvert.size()] = '\0';
	return writable;
}

string getWordAtIndexWithDlm(string  sentance, int index, const char* delimiter){
	char* sentanceInChar = getCharFromString(sentance);
	char* convertor = strtok(sentanceInChar, delimiter);
	for(int i = 0; i < index ; i++){
		convertor = strtok(NULL, delimiter);
		cout << convertor << endl;
	}
	string toReturn(convertor);
	if(toReturn.empty()){
		toReturn = "p";
	}

	return toReturn;
}


Broker::Broker(TCPSocket* peer1, TCPSocket* peer2, BrokerMng* parent){
	this->parent = parent;
	peerOne = peer1;
	peerTwo = peer2;
	listener = new MTCPListener();
	listener->add(peerOne);
	listener->add(peerTwo);

}

Broker::~Broker() {
	delete listener;
}

void Broker::run(){
	const vector<TCPSocket *> & sockets = listener->socketsList();
	while (true )
	{
		if ( sockets.empty() )
		{
			cout << "Broker waiting 1 second" << endl;
			usleep( 1000000 );
			continue;
		}
		//cout << "Dispatcher selecting " << sockets.size() << " sockets " << endl;
		TCPSocket * readySocket = listener->listen();
		if ( readySocket ) {
			cout << "Broker socket ready" << endl;
			char command[4];
			int length = readFromSocket(readySocket, command, sizeof(command));
			if ( length == 0 )
			{
				cout << "socket error. disconnecting client" << endl;
				break;
			}
			int commandType = ntohl( *( (int * ) command ) );
			//string toSend = new string();
			//strToChar(command);
			cout << "received command from " << readySocket->fromAddr() << ":" << readySocket->peerPort() << endl;
			// TODO if command length is 0, close the peer and remove it from listener

			//cout << "this is the sent command" << toSend << endl;
			if ( ! handleMessage(readySocket, command) ){
				break;
			}
			// TODO : parse and handle the commands : disconnect, open session...
		}
	}
	cout << "Broker terminated" << endl;
	sendCommandToClient(peerOne, CLOSE_SESSION_WITH_PEER, NULL);
	sendCommandToClient(peerTwo, CLOSE_SESSION_WITH_PEER, NULL);
	parent->rleasePeer(peerOne);
	parent->rleasePeer(peerTwo);
	parent->deleteBroker(this);
}

bool Broker::handleMessage(TCPSocket * socket, const char * commandStr) {
	cout <<"WE ARE IN HANDLEMSG OF BROKER " << endl;
	int command = 0;
	command = ntohl( *( (int * ) commandStr ) );
	cout <<"got command " << command << endl;
	string strngMsgToSnd;
	const char* msgToSnd;
	switch(command) {
	case SEND_MSG_TO_PEER:
	{
		char messageLength[4];
		readFromSocket(socket, messageLength, sizeof(messageLength));
		int dataLength = ntohl( *( (int * ) ( messageLength ) ) );
		char dataBuffer[dataLength];
		readFromSocket(socket, dataBuffer, sizeof(dataBuffer));
		string data(dataBuffer, dataLength);
		cout << "got message length " << dataLength << " command " << data.c_str() << endl;
		TCPSocket * socketToSnd = getOtherPeer(socket);
		sendCommandToClient(socketToSnd, SEND_MSG_TO_PEER, data.c_str());
		cout <<"We sent the command to client with " <<  data.c_str()  << endl;
		break;
	}
	case CLOSE_SESSION_WITH_PEER:
	{
		/**BY RETURNING FALSE WE ACTUALLY DO THIS:
		TCPSocket* otherPeer = getOtherPeer(socket);
		sendCommandToClient(socket, CLOSE_SESSION_WITH_PEER, NULL);
		sendCommandToClient(otherPeer, CLOSE_SESSION_WITH_PEER, NULL);
		 **/
		return false;
	}
	case EXIT:{
		sendCommandToClient(socket, EXIT, NULL);
		return false;
	}
	case SESSION_REFUSED: break;
	case SESSION_ESTABLISHED: break;
	case OPEN_SESSION_WITH_PEER :
	{
		sendCommandToClient(socket, SEND_MSG_TO_PEER, "Please end current conversation before starting a new one");
		break;
	}
	default:
	{
		sendCommandToClient(socket, SEND_MSG_TO_PEER, "Your request was not identified, Please try again");
		break;

	}
	return true;
	}
}

int Broker::readFromSocket(TCPSocket * socket,  char * buffer , int bytesToRead)
{
	int length = socket->recv(buffer, bytesToRead );
	if ( length != bytesToRead )
	{
		cout << "problem reading length " << length << endl;
		// TOOD this is error close the socket
	}
	return length;
}

int Broker::sendCommandToClient(TCPSocket * socket, int cmd,const char* buff){
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


TCPSocket* Broker::getOtherPeer(TCPSocket* socket){
	if(socket == peerOne){
		return peerTwo;
	}
	return peerOne;
}







} /* namespace npl */
