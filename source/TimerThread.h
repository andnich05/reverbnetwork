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