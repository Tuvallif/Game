/*
 * TCPSocket.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */


#include "TCPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace std;
using namespace npl;

TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr){
	this->serverAddr =  serverAddr;
	this->peerAddr =  peerAddr;
	socket_fd = connected_sock;
	local_port = 0;
}


TCPSocket::TCPSocket(int port){
	/**
	 * int socket(int domain, int type, int protocol);
	 * creates a TCP socket
	 * AF_INET - IPv4 Internet protocols
	 * SOCK_STREAM - TCP
	 * 0 - default protocol type
	 */
	socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);

	int optval = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


	local_port = 0;
	// clear the s_in struct
	bzero((char *) &serverAddr, sizeof(serverAddr));  /* They say you must do this    */

	//sets the sin address
	serverAddr.sin_family = (short)AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
	serverAddr.sin_port = htons((u_short)port);

	//bind the socket on the specified address
	printf("TCP server binding...\n");
	if (bind(socket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror ("Error naming channel");
	}
}


TCPSocket::TCPSocket(const string& peerIp, int port){
	//cout<<"openning new client socket"<<endl;

	/**
	 * int socket(int domain, int type, int protocol);
	 * creates a TCP socket
	 * AF_INET - IPv4 Internet protocols
	 * SOCK_STREAM - TCP
	 * 0 - default protocol type
	 */

	local_port = 0;
	socket_fd = ::socket (AF_INET, SOCK_STREAM, 0);

	// clear the s_in struct
	bzero((char *) &peerAddr, sizeof(peerAddr));  /* They say you must do this    */

	//sets the sin address
	peerAddr.sin_family = (short)AF_INET;
	peerAddr.sin_addr.s_addr = inet_addr(peerIp.data());
	peerAddr.sin_port = htons((u_short)port);

	int i = connect(socket_fd, (struct sockaddr *)&peerAddr, sizeof(peerAddr));
	if (i < 0)
	{
		perror ("Error establishing communications");
		::close(socket_fd);
	}
}


TCPSocket* TCPSocket::listenAndAccept(){
	int rc = listen(socket_fd, 1);
	if (rc<0){
		return NULL;
	}
	socklen_t len = sizeof(peerAddr);
	bzero((char *) &peerAddr, sizeof(peerAddr));

	int connect_sock = accept(socket_fd, (struct sockaddr *)&peerAddr, &len);
	cout << "Accepted from " << inet_ntoa(peerAddr.sin_addr) << ":" << peerAddr.sin_port << endl;
	return new TCPSocket(connect_sock,serverAddr,peerAddr);
}


int TCPSocket::recv(char* buffer, int length){
	return read(socket_fd,buffer,length);
}


int TCPSocket::send(const string& msg){
	return ::write(socket_fd,msg.c_str(),msg.size());
}

int TCPSocket::write(const char* buffer, int len){
	return ::write(socket_fd,buffer,len);
}


void TCPSocket::close(){
	//cout<<"closing socket"<<endl;
	shutdown(socket_fd,SHUT_RDWR);
	::close(socket_fd);
}


string TCPSocket::fromAddr(){
	return inet_ntoa(peerAddr.sin_addr);
}

int TCPSocket::peerPort(){
	return peerAddr.sin_port;
}

int TCPSocket::socket(){
	return socket_fd;
}

int TCPSocket::getPort(){
	unsigned int addrlen = sizeof(peerAddr);
	if(getsockname(socket_fd, (struct sockaddr *)&peerAddr, &addrlen) == 0 &&
		peerAddr.sin_family == AF_INET &&
	   addrlen == sizeof(peerAddr))
	{
	    local_port = ntohs(peerAddr.sin_port);
	}

	return local_port;
}

