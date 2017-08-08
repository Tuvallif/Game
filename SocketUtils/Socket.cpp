/*
 * Socket.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#include "Socket.h"
using namespace std;
namespace npl{

int socket_fd;
Socket::Socket() {

}


int Socket::bind(int port){
	struct sockaddr_in s_in;
	bzero((char *) &s_in, sizeof(s_in));
	s_in.sin_family = (short) AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_in.sin_port = htons(port);
	int rc = ::bind(socket_fd, (struct sockaddr *)&s_in, sizeof (s_in));
	if (rc <0){
	cout << "Error naming channel" << endl;
	}
	return rc;
}

int Socket::connect(const string& ip, int port){
return -1;
}
void Socket::close(){
}
/*int Socket::read(char* buffer, int length){
}
int Socket::write(const string& msg){
}*/
string Socket::fromAddr(){
	return inet_ntoa(this->peerAddr.sin_addr);
}
Socket::~Socket() {

}

};
