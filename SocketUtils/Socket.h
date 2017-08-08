/*
 * Socket.h
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#ifndef SOCKET_H_
#define SOCKET_H_
#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "FileInterface.h"
using namespace std;
namespace npl{
class Socket: public FileInterface {
protected:

	struct sockaddr_in peerAddr;
	int socket_fd;

public:
	Socket();
	virtual int bind(int port);
	virtual int connect(const string& ip, int port);
	virtual void close();
/*	virtual int read(char* buffer, int length);
	virtual int write(const string& msg);*/
	virtual string fromAddr();
	virtual ~Socket();
};
}
#endif /* SOCKET_H_ */
