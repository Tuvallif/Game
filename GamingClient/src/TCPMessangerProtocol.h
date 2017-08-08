/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_
#include "MThread.h"

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2
#define EXIT						3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define LOGIN_REGISTER 				7
#define LOGIN_REGISTER_SUCCESFULL   8
#define LOGIN_REGISTER_FAILED       9
#define GET_LIST_USERS             10
#define START_MATCH                11
#define SEEK                       12
#define MATCH_ACCEPTED             13
#define MATCH_REJECTED             14
#define REQUEST_START_MATCH        15
#define ACCEPT_REQUEST_MATCH       16
#define DECLINE_START_MATCH        17
#define GAME_STARTED               18

#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"

#define USER_NAME_SEPERATOR        ";"

#endif /* TCPMESSENGERPROTOCOL_H_ */
