/*
 * FileInterface.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: user
 */

#include "FileInterface.h"

namespace npl {

FileInterface::FileInterface() {
	// TODO Auto-generated constructor stub

}

FileInterface::~FileInterface() {
	// TODO Auto-generated destructor stub
}

void FileInterface::close(){
	cout << "closing socket" << endl;
	shutdown(socket_fd, SHUT_RDWR);
	::close(socket_fd);
}
int FileInterface::read(char* buffer, int length){
	return ::read(socket_fd,buffer, length);
	};
int FileInterface::write(const char* buffer, int length){
	return ::write(socket_fd,buffer,length);
	}

} /* namespace npl */
