
#include <iostream>
#include <assert.h>
#include "TCPMSNClient.h"
#include <stdlib.h>
#include <string>

using namespace std;
using namespace npl;

string curWaitingUser = "NULL";

void printInstructions(){
	cout<<"To open connect to server type: c <server ip>"<<endl;
	cout<<"To register or login to server type: l <user name>:<password>"<<endl;
	cout<<"To get list of online users type: li "<<endl;
	cout<<"To start a match with a specific user type: m <user name> "<<endl;
	cout<<"To start a match with a random user type: se "<<endl;
	cout<<"To open session type with peer: o <peer ip:port>"<<endl;
	cout<<"After the session is opened, to send message type: s <message>"<<endl;
	cout<<"To close opened session type: cs"<<endl;
	cout<<"To disconnect from server type: d"<<endl;
	cout<<"To exit type: x"<<endl;
}


int test() {
	cout<<"Welcome to TCP messenger TEST"<<endl<<flush;
	//printInstructions();

	TCPMSNClient* m1 = new TCPMSNClient();
	TCPMSNClient* m2 = new TCPMSNClient();


	// connect to server
	string ip = "127.0.0.1";
	cout << "connecting to server" << ip << endl;
	assert ( m1->connect(ip) );
	assert ( m2->connect(ip) );

	cout << "getting client2 port " << endl;
	int port = m2->getPort();
	cout << "client1 opening session with " << port << endl;
	m1->openSession(ip, port);

	string msg1 = "Hello client2!How are you? ";
	cout << "this is message from client 1 to client 2 " << msg1 << endl;
	m1->sendMessage(msg1);


	string msg2 = "Hello to you client1! I am good, and you? ";
	cout << "this is message from client 2 to client 1 " << msg2 << endl;
	m2->sendMessage(msg2);

	//m1 ending the session
	cout << "now session will end" << endl;
	m1->closeSession();

	//m2 disconnecting
	cout << "now client2 connection will end" << endl;
	m2->disconnect();

	//m1 disconnecting
	cout << "now client1 connection will end" << endl;
	m1->disconnect();

	return 0;



}

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


int main() {
	cout<<"Welcome to Gaming application"<<endl<<flush;
	printInstructions();
	TCPMSNClient* client = new TCPMSNClient();

	while(true) {
		string msg;
		string command, commandType;
		cout<< "type your command:" << endl;
		std::getline (std::cin,command);
		//commandType = getFirstWord(command);
		commandType = getWordAtIndexWithDlm(command, 0, " ");

		//choosing what to do according to input
		//making a connection
		if(commandType == "c"){
			string ip;
			//getting ip and port <ip>:<port>
			ip = getWordAtIndexWithDlm(command, 1, " ");

			client->connect(ip);

		}
		// open session
		else if(commandType == "o"){
			string ip;
			string portInString;
			//getting ip and port <ip>:<port>
			ip = getWordAtIndexWithDlm(command, 1, " ");
			//getting the port before changing ip
			portInString = getWordAtIndexWithDlm(ip, 1, ":");
			//getting only ip
			ip = getWordAtIndexWithDlm(ip, 0, ":");
			int port = atoi(portInString.c_str() );
			client ->openSession(ip, port);
		}
		// login
		else if(commandType == "l"){
			string usernameAndPassword;
			string username, password;
			//getting username and password <username>:<password>
			usernameAndPassword = getWordAtIndexWithDlm(command, 1, " ");
			username = getWordAtIndexWithDlm(usernameAndPassword, 0, ":");
			password = getWordAtIndexWithDlm(usernameAndPassword, 1, ":");
			cout <<"trying to login with user name: "<< username << " and password " << password <<endl;
			//getting only ip
			client->loginRegister(username, password);
		}
		//get list of users
		else if(commandType == "li"){
			client->getListOfUsers();
		}
		//start a match with user
		else if(commandType == "m"){
			string userName;
			//getting ip and port <ip>:<port>
			userName = getWordAtIndexWithDlm(command, 1, " ");
			client->startMatchWithUser(userName);
		}

		else if(commandType == "an"){
			string answer = getWordAtIndexWithDlm(command, 1, " ");

		}
		//start a match with user
		else if(commandType == "se"){
			client->seek();
		}
		//send a message
		else if(commandType == "s"){
		msg = command.substr(2, command.length());
		//msg is GOOD
		if(msg.size() > 0 && msg[0] == ' ') msg.erase(0,1);
			client->sendMessage(msg);
		}
		//close session
		else if(commandType == "cs"){
			cout<< "Closing the session..." << endl;
			client->closeSession();
		}
		//disconnect
		else if(commandType == "d"){
			cout<< "Disconnecting from the server..." << endl;
			client->disconnect();
		}
		//exit
		else if(commandType == "x"){
			cout<< "Goodbye!" << endl;
			break;
		}
		else if(commandType == "r")
		{
			string answer = getWordAtIndexWithDlm(command, 1, " ");
			client->answerRequest(answer);
		}
		//wrong input - handeled
			else if(commandType == "p"){
			cout<< "There was a problem!!!" << endl;
			printInstructions();
		}

		//wrong input unhandeled
		else{
			cout<<"wrong input"<<endl;
			printInstructions();
		}
	}
	//client->disconnect();
	client->disconnect();
	//delete client;
	delete client;
	cout<<"client was closed"<<endl;
	return 0;
}


int main1() {
	test();
}


