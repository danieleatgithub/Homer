/*
 * Calendar.cpp
 *
 *  Created on: 28/feb/2016
 *      Author: daniele
 */


#include "Calendar.h"

using namespace std;

namespace calendar {

// Singleton object threadsafe
Calendar*  Calendar::pInstance = nullptr;
mutex Calendar::sMutex;

// Calendar
mutex Calendar::__wait_sem;
thread Calendar::scheduler_t;
atomic_bool stop_scheduler;

// Event queues

EventQ wait_q;
EventQ run_q;



Calendar& Calendar::get_instance() {
	static Cleanup cleanup;
	lock_guard<mutex> guard(sMutex);
	if (pInstance == nullptr)
		pInstance = new Calendar();
	return pInstance;
}

void Calendar::start() {
	lock_guard<mutex> guard(sMutex);
	stop_scheduler = false;
	scheduler_t = std::thread(&Calendar::wait_scheduler,this);
}

void Calendar::stop() {
	lock_guard<mutex> guard(sMutex);
	stop_scheduler = true;
	__wait_sem.unlock();
	scheduler_t.join();
}

void Calendar::eventMove(CalendarEvent &event, Calendar::EventQ &from, Calendar::EventQ &to ) {
	from.eraseEvent(event);
	to.addEvent(event);
}

void Calendar::eventExec(CalendarEvent &event) {
	std::thread([&] {
			eventMove(event,wait_q,run_q);
			event._handler();
			event.after(std::chrono::seconds(10)); // FIXME recalculate event
			eventMove(event,run_q,wait_q);
		}).detach();
}


bool Calendar::addEvent(CalendarEvent &event) {
	return(wait_q.addEvent(event));
}

void Calendar::wait_scheduler() {
	CalendarEvent event;
	while(!stop_scheduler) {
		event = wait_q.waitEvent();
		eventExec(event);
	}
}

bool Calendar::EventQ::addEvent(CalendarEvent& event) {
	{
		lock_guard<mutex> lg(q_mutex);
		e_queue.push_back(event);
		sort(e_queue.begin(),e_queue.end());
	}
	q_cond.notify_all();
}

bool Calendar::EventQ::eraseEvent(CalendarEvent& event) {
	lock_guard<mutex> lg(q_mutex);
	auto i = find(e_queue.begin(),e_queue.end(),event);
	if(i == e_queue.end()) 	return false;
	e_queue.erase(i);
	return true;
}

CalendarEvent& Calendar::EventQ::waitEvent() {
	unique_lock<mutex> ul(q_mutex);
	if(e_queue.empty()) {
		q_cond.wait(ul,[]{ return !e_queue.empty(); });
	}
	// FIXME erase could be enter here
	if(q_cond.wait_until(ul,e_queue.front().dead_line) == std::cv_status::timeout){
		return e_queue.front();
	} else {
		return nullptr;
	}
}

Calendar::Cleanup::~Cleanup() {
	lock_guard<mutex> guard(Calendar::sMutex);
	delete Calendar::pInstance;
	Calendar::pInstance = nullptr;
}

Calendar::Calendar() {
	stop_scheduler = false;
	__wait_sem.unlock();
}

Calendar::~Calendar() {
	stop();
}

}
