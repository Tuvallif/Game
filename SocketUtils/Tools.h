/*
 * Tools.h
 *
 *  Created on: Aug 5, 2017
 *      Author: user
 */

#ifndef TOOLS_H_
#define TOOLS_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>


using namespace std;

namespace npl {


class Tools {
public:

	Tools();

	virtual ~Tools();

	char* getCharFromString(string toConvert);

	string getWordAtIndexWithDlm(string  sentance, int index, const char* delimiter);


};

} /* namespace npl */

#endif /* TOOLS_H_ */
