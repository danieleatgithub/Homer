/*
 * Calendar.h
 *
 *  Created on: 26/feb/2016
 *      Author: daniele
 */

#ifndef CALENDAR_H_
#define CALENDAR_H_
#include <ctime>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

namespace calendar {


using cb_t = std::function<void()>;


class CalendarEvent {
public:

	std::chrono::system_clock::time_point dead_line;
	cb_t _handler;

	bool operator<(const CalendarEvent& b) const {
		return(this->dead_line < b.dead_line);
	}
	bool operator==(const CalendarEvent& b) const {
		return(this->dead_line == b.dead_line);
	}
	void after(chrono::seconds sec) {
		dead_line = std::chrono::system_clock::now() + sec;
	}
};


class Calendar {

	friend class Calendar::Cleanup {
	public:
		~Cleanup();
	};

	friend class Calendar::EventQ {
	public:
		vector<CalendarEvent> e_queue;
		mutex m;
		mutex q_mutex;
		condition_variable q_cond;
		CalendarEvent& waitEvent();
		bool addEvent(CalendarEvent& event);
		bool eraseEvent(CalendarEvent& event);

	};

public:
	static Calendar& get_instance();

	void start();
	void stop();
	bool addEvent(CalendarEvent& event);

protected:
	static Calendar* pInstance;

private:
	static Calendar::EventQ wait_q;
	static Calendar::EventQ run_q;
	static atomic_bool stop_scheduler;
	static mutex __wait_sem;
	static thread scheduler_t;

	Calendar();
	virtual ~Calendar();
	static mutex sMutex;

	void eventMove(CalendarEvent &event, Calendar::EventQ &from, Calendar::EventQ &to);
	void eventExec(CalendarEvent &event);
	void wait_scheduler();


};

} /* namespace calendar */



#endif /* CALENDAR_H_ */
