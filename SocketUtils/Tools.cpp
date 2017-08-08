/*
 * Tools.cpp
 *
 *  Created on: Aug 5, 2017
 *      Author: user
 */

#include "Tools.h"



using namespace std;

namespace npl{

Tools::Tools() {
	// TODO Auto-generated constructor stub

}

Tools::~Tools() {
	// TODO Auto-generated destructor stub
}

char* Tools::getCharFromString(string toConvert){
	char * writable = new char[toConvert.size() + 1];
	std::copy(toConvert.begin(), toConvert.end(), writable);
	writable[toConvert.size()] = '\0';
	return writable;
}





string Tools::getWordAtIndexWithDlm(string  sentance, int index, const char* delimiter){
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

} /* namespace npl */
