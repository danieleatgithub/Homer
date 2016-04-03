/*
 * Scheduler.hpp
 *
 *  Created on: 27/mar/2016
 *      Author: daniele
 */


#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include <IDGenerator.h>
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdbool>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

using namespace std;

namespace shd {

using cb_t = std::function<void()>;

class Scheduler;

/**
 * Implements a task managed by Scheduler
 *
 */

class Task {
  friend class Scheduler;


  public:
	/**
	 *
	 * @return Current deadline
	 */
    const chrono::system_clock::time_point& getTime() const {
        return time;
    }
    /**
     * Set deadline
     * @param time deadline
     */
    void setTime(const chrono::system_clock::time_point& time) {
        this->time = time;
    }
	/**
	 * Get the callback function
	 * @return callback
	 */
    const cb_t& getCallback() const {
        return callback;
    }
    const uint32_t& getId() const {
    	return id;
    }
    /**
     * Set the callback function
     * @param callback
     */
    void setCallback(const cb_t& callback) {
        this->callback = callback;
    }
	const chrono::system_clock::duration& getInterval() const {
		return interval;
	}

	void setInterval(const chrono::system_clock::duration& interval) {
		this->interval = interval;
	}
     /**
     *
     * Default constructor with a disabled task available only
     * for friends class
     */
    Task() {
    	this->id =  IDGenerator::get_istance().getId();
    	this->time =  chrono::system_clock::now();
        this->callback = 0;
        this->interval = chrono::seconds(0);;
        this->cancelled = false;
    }
    /**
     * Constructor with callback
     * @param callback
     */
    Task(cb_t callback) {
    	this->id =  IDGenerator::get_istance().getId();
    	this->time =  chrono::system_clock::now();
         this->callback = callback;
         this->interval = chrono::seconds(0);
         this->cancelled = false;
     }

    Task(chrono::system_clock::duration interval,cb_t callback) {
    	this->id =  IDGenerator::get_istance().getId();
    	this->time =  chrono::system_clock::now() + interval;
    	 this->interval = interval;
         this->callback = callback;
         this->cancelled = false;
      }

    ~Task() {
    }
    friend bool operator==(const Task& lhs, const Task& rhs) {
        return (lhs.id == rhs.id);
    }
    friend bool operator<(const Task& lhs, const Task& rhs) {
        return (lhs.time < rhs.time);
    }
    friend std::ostream& operator<<(std::ostream &strm, const Task &task) {
       return strm << "ID=" << task.id << ",TIME=" << chrono::system_clock::to_time_t(task.time);
     }

  private:
    cb_t callback;
    chrono::system_clock::duration interval;
    chrono::system_clock::time_point time;
    uint32_t id;
    bool cancelled;

	bool isCancelled() const {
		return cancelled;
	}

	void setCancelled(bool cancelled) {
		this->cancelled = cancelled;
	}

    /**
     * Thread launched in detached mode
     */
    void go() const {

        if(callback) {
        	std::thread( callback ).detach();
        }
    }
};

/**
 * Implements a simple task scheduler suspended on the earliest task, minimum interval is second
 * Tasks are spawned in detached separeted thread.
 * The same event could be scheduled only one time
 *
 * Examples:
 *
 * 	 Scheduler sch;
 *
 *   Task t1(std::chrono::seconds(2),foo_task1);
 *   Task t2(bind_fun2);
 *   Task t3(bind_fun3);
 *   Task t4([] () {
 *       foo_task4();
 *   });
 *
 *   Task t5([&] { foo_promise(ref(promise_1),50); });
 *   Task t6([&] { foo_promise(ref(promise_2),60); });
 *   Task t7([&sch] { foo_sleep(7); });
 *
 *   auto now = std::chrono::system_clock::now();
 *   sch.ScheduleEvery(t1);
 *   sch.ScheduleEvery(std::chrono::seconds(1),t2);
 *   sch.ScheduleEvery(std::chrono::seconds(4),t3);
 *   sch.ScheduleCancel(t2);
 *   sch.ScheduleAt(now + std::chrono::seconds(10), t4);
 *   sch.ScheduleAt(now + std::chrono::seconds(6), t5);
 *   sch.ScheduleAt(now + std::chrono::seconds(12), [&] { sch.ScheduleCancel(t3); });
 *   sch.ScheduleAt(now + std::chrono::seconds(13), [&] {
 *   	cout << "TRACE RESCHEDULE" << endl;
 *   	sch.ScheduleEvery(std::chrono::seconds(4),t1);
 *   });
 *   sch.ScheduleAt(now + std::chrono::seconds(5), [&sch] { foo_chain_task2(sch); });
 *   sch.ScheduleAfter(std::chrono::seconds(5), [] () { std::cout << "TRACE hello" << endl; });
*/

class Scheduler {

  public:

    Scheduler()
        :go_on(true) {
        loop_thread.reset(new std::thread([this]() {
            this->ThreadLoop();
        }));
    }
    ~Scheduler() {
        go_on = false;
        ScheduleAt(chrono::system_clock::now(),[] () {});
        loop_thread->join();
    }


    Scheduler& operator=(const Scheduler& rhs) = delete;
    Scheduler(const Scheduler& rhs) = delete;
    friend std::ostream& operator<<(std::ostream &strm, const Scheduler &shd) {
    	for(auto it = shd.waiting_tasks.begin(); it != shd.waiting_tasks.end(); it++) {
    		strm << *it << ";";
    	}
    	return strm;
     }
    unsigned int getWaitingSize() {
    	return waiting_tasks.size();
    }
    unsigned int getRunningSize() {
    	return running_tasks.size();
    }
    void ScheduleAt(chrono::system_clock::time_point time, Task& task) {
        std::unique_lock<std::mutex> lock(mutex);
        if(!task.isCancelled()) {
			task.setTime(time);
	        task.setCancelled(true);
	        auto tasks_it = find_if (waiting_tasks.begin(), waiting_tasks.end(), [&] (const Task& element) { return(task == element); });
	        if(tasks_it != waiting_tasks.end()) {
	        	if(tasks_it == waiting_tasks.begin()) {
	        		waiting_tasks.erase(*tasks_it);
	        		blocker.notify_one();
	        	} else {
	        		waiting_tasks.erase(*tasks_it);
	        	}
	        }
			task.setCancelled(false);
			waiting_tasks.insert(task);

			 // if task has the shortest deadline unlock the scheduler
			if (*(waiting_tasks.begin()) == task)
				blocker.notify_one();
			}
    }
    void ScheduleAt(chrono::system_clock::time_point time, cb_t callback) {
        Task e(callback);
        ScheduleAt(time,e);
    }
    void ScheduleEvery(Task& task) {
        cb_t callback = task.getCallback();
        cb_t waitFunc = [this,callback,&task]() {
             callback();
             task.setCallback(callback);
            this->ScheduleEvery(task);
        };
        task.setCallback(waitFunc);
        ScheduleAt(chrono::system_clock::now() + task.getInterval(),task);
    }
   void ScheduleEvery(chrono::system_clock::duration interval, Task& task) {
        cb_t callback = task.getCallback();
        cb_t waitFunc = [this,interval,callback,&task]() {
            callback();
            task.setCallback(callback);
            this->ScheduleEvery( interval, task);
        };
        task.setCallback(waitFunc);
        ScheduleAt(chrono::system_clock::now() + interval,task);
    }
    void ScheduleCancel(Task& task) {
        std::unique_lock<std::mutex> lock(mutex);
        task.setCancelled(true);
        auto it = find_if (waiting_tasks.begin(), waiting_tasks.end(), [&] (const Task& element) { return(task == element); });
        if(it != waiting_tasks.end()) {
        	if(it == waiting_tasks.begin()) {
        		waiting_tasks.erase(*it);
        		blocker.notify_one();
        	} else {
        		waiting_tasks.erase(*it);
        	}
        }
    }
    void ScheduleAfter(chrono::system_clock::duration interval, Task& task) {
    	this->ScheduleAt(chrono::system_clock::now(),task);
    }
    void ScheduleAfter(chrono::system_clock::duration interval, cb_t callback) {
    	Task e(callback);
    	this->ScheduleAt(chrono::system_clock::now(),e);
    }

  private:
    std::set<Task> waiting_tasks;
    std::set<Task> running_tasks;
    std::mutex mutex;
    std::unique_ptr<std::thread> loop_thread;
    std::condition_variable blocker;
    bool go_on;

    void ThreadLoop() {
        while(go_on) {
            {
                std::unique_lock<std::mutex> lock(mutex);
                auto now = std::chrono::system_clock::now();
                auto cur = waiting_tasks.begin();
                if ( waiting_tasks.empty()==false && (*cur).getTime() <= now && (*cur).isCancelled()== false) {
                	auto ins = running_tasks.insert(*cur);
                    waiting_tasks.erase(*cur);
                	cur = ins.first;
                	(*cur).go();
                	running_tasks.erase(*cur);
                 }
//                cout << "LOOP " << tasks.size() << " SCHEDULER " << *this << endl;
//                cout << "LOOP" << waiting_tasks.size() << "-" << running_tasks.size() << endl;
                if (waiting_tasks.empty())
                    blocker.wait(lock);
                else
                    blocker.wait_until(lock, (*cur).getTime());
            }
        }
    }

};

}



#endif /* SCHEDULER_HPP_ */
