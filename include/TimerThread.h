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

#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <thread>

class TimerThread {
public:
	//TimerThread(long int interval = 1000, bool singleShot = true);
	TimerThread();
	//template <typename F> void setCallbackFunction(F callbackFunction);
	//template <typename F> void start(F callbackFunction);
	typedef std::function<void(void)> VoidFunction;
	void start(const long int& interval, const VoidFunction functionToCall);
	void stop();
	//inline void setSingleShot(const bool& singleShot = true) { this->singleShot = singleShot; }
	//inline bool isSingleShot() { return singleShot; }
	//inline void setInterval(const long int& interval) { this->interval = interval; }
	//inline long int getInterval() { return interval; }

private:
	std::thread thread;
	/*long int interval;
	bool singleShot;*/
	bool running;
};


#endif // TIMERTHREAD_H