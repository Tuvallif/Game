/*
 * MThread.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef MTHREAD_H_
#define MTHREAD_H_

#include <pthread.h>

namespace npl{
class MThread{
public:
	pthread_t threadId;

public:
	virtual void run() = 0;
	virtual void start();
	void waitForThread();
	virtual ~MThread();
};

}
#endif /* MTHREAD_H_ */
