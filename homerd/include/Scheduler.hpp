/*
 * Scheduler.hpp
 *
 *  Created on: 27/mar/2016
 *      Author: daniele
 */


#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include <functional>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <memory>
#include <map>
#include <queue>
#include <atomic>
#include <algorithm>

using namespace std;

namespace shd {

using cb_t = std::function<void()>;

class Event {
  public:
    const cb_t& getCallback() const {
        return callback;
    }

    void setCallback(const cb_t& callback) {
        this->callback = callback;
    }

    const chrono::system_clock::time_point& getTime() const {
        return time;
    }
    void setTime(const chrono::system_clock::time_point& time) {
        this->time = time;
    }
    Event() {
        this->callback = 0;
        this->time = chrono::system_clock::now();
        this->id = this;
    }
    Event(cb_t callback) {
        this->callback = callback;
        this->time = chrono::system_clock::now();
        this->id = this;
    }
    friend bool operator==(const Event& lhs, const Event& rhs) {
        return (lhs.id == rhs.id);
    }
    friend bool operator<(const Event& lhs, const Event& rhs) {
        return (lhs.time > rhs.time);
    }
    void go() const {
        if(callback)
            std::thread( callback ).detach();
    }
  private:
    chrono::system_clock::time_point time;
    cb_t callback;
    void *id;
};

class Scheduler {
  private:
    std::vector<Event> tasks;
    std::mutex mutex;
    std::unique_ptr<std::thread> thread;
    std::condition_variable blocker;
    atomic<int> counter;
    Event event_now;
    bool go_on;

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
    int getCounter() {
        return counter;
    }
    void ScheduleAt(chrono::system_clock::time_point time, Event& event) {
        std::unique_lock<std::mutex> lock(mutex);
        event.setTime(time);
        tasks.push_back(event);
        sort (tasks.begin(),tasks.end());
        // if task has the shertest deadline unlock the scheduler
        if (event == tasks.back())
            blocker.notify_one();
    }
    void ScheduleAt(chrono::system_clock::time_point time, cb_t callback) {
        Event e(callback);
        ScheduleAt(time,e);
    }

    void ScheduleEvery(chrono::system_clock::duration interval, Event& event) {
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
    void ScheduleCancel(Event& event) {
        std::unique_lock<std::mutex> lock(mutex);
        auto it = find(tasks.begin(),tasks.end(),event);
        if(it != tasks.end()) {
            if(event == tasks.back()) {
                blocker.notify_one();
            }
            tasks.erase(it);
        }
    }
    void ScheduleAfter(chrono::system_clock::duration interval, Event& event) {
    	this->ScheduleAt(chrono::system_clock::now(),event);
    }

};

}



#endif /* SCHEDULER_HPP_ */
