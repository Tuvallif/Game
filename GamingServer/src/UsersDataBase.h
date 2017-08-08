/*
 * UsersDataBase.h
 *
 *  Created on: Jul 13, 2017
 *      Author: user
 */

#ifndef USERSDATABASE_H_
#define USERSDATABASE_H_

#include <map>
#include <list>
#include <string>
#include "TCPSocket.h"

using namespace std;

namespace npl {

class User {
public:
	string username;
	TCPSocket * socket;
	static const int STATE_DEFAULT = 0;
	static const int STATE_SEEKING = 1;
	static const int STATE_PLAYING = 2;
	static const int STATE_BUSY = 3;

	int state;
};

class UsersDataBase {
public:
	UsersDataBase( const string  & databaseFileName );
	virtual ~UsersDataBase();

	/**
	 * try to login the user.if the user does not exist register it
	 * return true if the user logged in successfully
	 */
	bool login( const string & username, const string & password, TCPSocket * socket );


	/**
	 * log out a user.
	 */
	void logout( const string & username );

	/**
	 * return list of connected users
	 */
	list< User > listUsers();

	/**
	 * return list of connected users in a string format
	 */
	string listUsersInString();

	/**
	 * this method checks for seeking users, if no seeking user available, changes the seeker state
	 */
	User* seek(TCPSocket* seekingSocket);

	/**
	 * This methos offers a player 2 to start a match with player 1 after player 1's request
	 */
	int checkAvilability(const string& userName);
	/**
	 * this returns user by socket num
	 */
	User* findUserBySocket(TCPSocket* socket);
	/**
	 * this returns user by name
	 */
	User* findUserByName(const string& userName);

private:

	/**
	 * gets the password of the user from the database
	 */
	bool getPassword( const string & username, string & password );

	/**
	 * append the username and password to the end of the file
	 * in the format username=password\n
	 */
	bool registerUser( const string & username, const string & password );

	/**
	 * this method receives a password and retirns a hashed password
	 */
	string hashPasswordString(const string & password );

	/**
	 * hash function
	 */
	unsigned hash_str(const char* s);
	/**
	 * This method changes the state of the user in the online user list
	 */
	void changeStateToSeek(TCPSocket* socketToChange);




	const string fileName;
	list< User > users;
};

} /* namespace npl */

#endif /* USERSDATABASE_H_ */
