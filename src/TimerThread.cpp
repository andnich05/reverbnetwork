/*
* TimerThread:
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// UNUSED

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