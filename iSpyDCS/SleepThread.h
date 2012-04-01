#ifndef SLEEPTHREAD_H_
#define SLEEPTHREAD_H_

#include <qthread.h>

class SleepThread : QThread {
public:
	static void msleep(unsigned long msecs) {
		QThread::msleep(msecs);
	}
};


#endif /* SLEEPTHREAD_H_ */
