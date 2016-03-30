/*
 * Scheduler.hpp
 *
 *  Created on: 27/mar/2016
 *      Author: daniele
 */


#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdbool>
#include <functional>
#include <iterator>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

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
    /**
     * Set the callback function
     * @param callback
     */
    void setCallback(const cb_t& callback) {
        this->callback = callback;
    }
    /**
     *
     * Default constructor with a disabled task available only
     * for friends class
     */
    Task() {
    	init();
        this->callback = 0;
    }
    /**
     * Constructor with callback
     * @param callback
     */
    Task(cb_t callback) {
    	init();
        this->callback = callback;
     }

  private:
    chrono::system_clock::time_point time;
    cb_t callback;
    /**
     * this as used as identifier and is carried through copies
     */
    void *id;

    void init() {
        this->time = chrono::system_clock::now();
        this->id = this;
    }
    /**
     * Compares id carried to find a scheduled task
     * @param lhs
     * @param rhs
     * @return
     */
    friend bool operator==(const Task& lhs, const Task& rhs) {
        return (lhs.id == rhs.id);
    }
    // FIXME: Rivedere in modo piu pulito
    friend bool operator<(const Task& lhs, const Task& rhs) {
        return (lhs.time > rhs.time);
    }
    /**
     * Thread launched in detached mode
     */
    void go() const {
    	printf("gooo %p %p\n",this,this->id);
        if(callback)
            std::thread( callback ).detach();
    }
};

/**
 * Implements a simple task scheduler suspended on the earliest task, minimum interval is second
 * Tasks are spawned in detached separeted thread.
 * The same event could be scheduled only one time FIXME: insert exception
 *
 * Examples:
 *
 *  Scheduler sch;
 *
 *  Task t1([&] { foo_promise(ref(promise_1),40); });
 *  Task t2([&] { foo_promise(ref(promise_2),60); });
 *  Task t3([&] { foo_sleep(4); });
 *
 *  sch.ScheduleEvery(std::chrono::seconds(2),t1);
 *  sch.ScheduleEvery(std::chrono::seconds(6),t2);
 *  sch.ScheduleCancel(t1);
 *  sch.ScheduleAt(now + std::chrono::seconds(1), t3);
 *  sch.ScheduleAfter(std::chrono::seconds(1), [] () { std::cout << "hello" << endl; });
 *
 *
 */
class Scheduler {

  public:

    Scheduler()
        :go_on(true) {
        counter = 0;
        event_now.setCallback([] () {});
        thread.reset(new std::thread([this]() {
            this->ThreadLoop();
        }));
    }
    ~Scheduler() {
        go_on = false;
        ScheduleAt(chrono::system_clock::now(),event_now);
        thread->join();
    }


    Scheduler& operator=(const Scheduler& rhs) = delete;
    Scheduler(const Scheduler& rhs) = delete;

    int getCounter() {
        return counter;
    }
    void ScheduleAt(chrono::system_clock::time_point time, Task& event) {
        std::unique_lock<std::mutex> lock(mutex);
        event.setTime(time);
        tasks.push_back(event);
        sort (tasks.begin(),tasks.end());
        // if task has the shertest deadline unlock the scheduler
        if (event == tasks.back())
            blocker.notify_one();
    }
    void ScheduleAt(chrono::system_clock::time_point time, cb_t callback) {
        Task e(callback);
        ScheduleAt(time,e);
    }
    void ScheduleEvery(chrono::system_clock::duration interval, Task& event) {
        cb_t callback = event.getCallback();
        cb_t waitFunc = [this,interval,callback,&event]() {
            counter++;
            callback();
            counter--;
            event.setCallback(callback);
            this->ScheduleEvery( interval, event);
        };
        event.setCallback(waitFunc);
        ScheduleAt(chrono::system_clock::now() + interval,event);
    }
    void ScheduleCancel(Task& event) {
        std::unique_lock<std::mutex> lock(mutex);
        auto it = find(tasks.begin(),tasks.end(),event);
        if(it != tasks.end()) {
            if(event == tasks.back()) {
                blocker.notify_one();
            }
            tasks.erase(it);
        }
    }
    void ScheduleAfter(chrono::system_clock::duration interval, Task& event) {
    	this->ScheduleAt(chrono::system_clock::now(),event);
    }
    void ScheduleAfter(chrono::system_clock::duration interval, cb_t callback) {
    	Task e(callback);
    	this->ScheduleAt(chrono::system_clock::now(),e);
    }

  private:
    std::vector<Task> tasks;
    std::mutex mutex;
    std::unique_ptr<std::thread> thread;
    std::condition_variable blocker;
    atomic<int> counter;
    Task event_now;
    bool go_on;

    void ThreadLoop() {
        while(go_on) {
            {
                std::unique_lock<std::mutex> lock(mutex);
                auto now = std::chrono::system_clock::now();
                if ( tasks.empty()==false && tasks.back().getTime() <= now) {
                    tasks.back().go();
                    tasks.pop_back();
                }
                if (tasks.empty())
                    blocker.wait(lock);
                else
                    blocker.wait_until(lock, tasks.back().getTime());
            }
        }
    }

};

}



#endif /* SCHEDULER_HPP_ */
