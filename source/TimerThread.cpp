#include "TimerThread.h"
#include <chrono>

// Source: http://stackoverflow.com/questions/21521282/basic-timer-with-stdthread-and-stdchrono

//TimerThread::TimerThread(long int interval, bool singleShot) 
//	: interval(interval), singleShot(singleShot), running(false) {
//
//}

TimerThread::TimerThread() {
	running = false;
}

//template <typename F> void TimerThread::setCallbackFunction(F callbackFunction) {
//	this->callbackFunction = callbackFunction;
//}

//template <typename F> void TimerThread::start(F callbackFunction) {
//	if (!running) {
//		thread = std::thread(callbackFunction) {
//			running = true;
//			while (true) {
//				std::this_thread::sleep_for(std::chrono::seconds(interval));
//			}
//		}
//	}
//}

//void TimerThread::stop() {
//	if (running) {
//		thread.join();
//		running = false;
//	}
//}

void TimerThread::start(const long int& interval, const VoidFunction functionToCall) {
	running = true;
	// Lambda explanation: http://goparallel.sourceforge.net/sharing-local-variables-c-lambda-functions/
	thread = std::thread([=]() {
		//while (running) {
		//	//functionToCall();
		//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//}
	});
}

void TimerThread::stop() {
	running = false;
	thread.join();
}