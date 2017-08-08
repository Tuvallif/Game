//============================================================================
// Name        : TCPMSNClientCLI.cpp
// Author      : Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include "TCPMSNClient.h"
#include <assert.h>

using namespace std;
using namespace npl;


int test2() {
	cout<<"Welcome to TCP messenger application"<<endl<<flush;
	//printInstructions();

	TCPMSNClient* m1 = new TCPMSNClient();
	TCPMSNClient* m2 = new TCPMSNClient();

	// connect to server
	string ip = "127.0.0.1";
	cout << "connecting to " << ip << endl;
	assert ( m1->connect(ip) );
	assert ( m2->connect(ip) );

	cout << "enter client2 port " << endl;
	int port;
	std::cin >> port;
	cout << "client1 opening session with " << port << endl;
	m1->openSession(ip, port);

	char msg[100];
	cout << "please enter m1 message" << endl;
	cin >> msg;
	m1->sendMessage(msg);

	cout << "please enter m2 message" << endl;
	cin >> msg;
	m2->sendMessage(msg);

	cout << "press any key to exit" << endl;
	char c;
	cin >> c;

	cout << "test passed" << endl;
	return 0;
}
