/*
 * FileInterface.h
 *
 *  Created on: Apr 4, 2016
 *      Author: user
 */

#ifndef FILEINTERFACE_H_
#define FILEINTERFACE_H_
#include <iostream>
#include <string.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
namespace npl {

class FileInterface {
protected:
	int socket_fd;

public:
	FileInterface();
	virtual ~FileInterface();
	virtual void close();
	virtual int read(char* buffer, int length);
	virtual int write(const char* buffer, int length);
};

} /* namespace npl */

#endif /* FILEINTERFACE_H_ */
