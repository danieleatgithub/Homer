/*
 * HomerMenu.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef HOMERMENU_HPP_
#define HOMERMENU_HPP_
#include <Menu.hpp>
#include <mutex>
#include <memory>
#include <Sysinfo.h>

namespace homerio {

/**
 * Homer Menu
 *
 */
class HomerMenu {
private:

    Task timedHome;
    shared_ptr<MenuComponent> active_element;
    KeyPanel &keyPanel;
    Scheduler& scheduler;
    Registration rkeyPanel, rWrDisplay;
    Sysinfo sysinfo = Sysinfo::get_instance();


	KeyButton nokey;
	std::mutex mutex;

	shared_ptr<SubMenu> root = std::make_shared<SubMenu>(string("root"));
	shared_ptr<MenuLeaf> welcome = std::make_shared<MenuLeaf>(string("Homer"),"IP:"+ sysinfo.get_local_ip("eth0"));
	shared_ptr<MenuLeaf> ciao = std::make_shared<MenuLeaf>(string("ciao"));
	shared_ptr<MenuLeaf> dany = std::make_shared<MenuLeaf>(string("dany"));
	shared_ptr<SubMenu> system = std::make_shared<SubMenu>(string("system"));
	shared_ptr<MenuLeaf> network = std::make_shared<MenuLeaf>(string("network"));
	shared_ptr<MenuLeaf> cpu = std::make_shared<MenuLeaf>(string("cpu"));
	shared_ptr<SubMenu> sensors = std::make_shared<SubMenu>(string("sensors"));
	shared_ptr<MenuLeaf> temperature = std::make_shared<MenuLeaf>(string("temperature"));
	shared_ptr<MenuLeaf> pressure = std::make_shared<MenuLeaf>(string("pressure"));

	MoveVisitor 	mv;
	vector<shared_ptr<MenuActionVisitor>> visitors;

	void leave(KeyButton& k) {
	    for(auto v : visitors) {
	    	active_element->exe_leave(*v,k);
	    }
	}
	void enter(KeyButton& k) {
	    for(auto v : visitors) {
	    	active_element->exe_enter(*v,k);
	    }
	}
	void click(KeyButton& k) {
	    for(auto v : visitors) {
	    	active_element->exe_click(*v,k);
	    }
	}


public:
    HomerMenu(KeyPanel& kpl, Scheduler& sch):  keyPanel(kpl), scheduler(sch), mv(root){

		root->add(welcome);
		root->add(ciao);
		sensors->add(temperature);
		sensors->add(pressure);
		system->add(network);
		system->add(cpu);
		system->add(sensors);
		root->add(system);
		root->add(dany);

		active_element = root->home();

		// Timer on no key press reset menu to Home
		timedHome.setCallback([&] () {
			leave(nokey);
			mutex.lock();
			active_element = mv.home();
			mutex.unlock();
			enter(nokey);
		});

		// Attach key handler
		keyPanel.key_attach(rkeyPanel, [&] ( KeyButton& k ) {
				scheduler.ScheduleCancel(timedHome);
				 if(k.get_key() == BUTTON_ENTER) {
					 click(k);
				 } else {
					 leave(k);
					 mutex.lock();
					 active_element = active_element->exe_move(mv,k);
					 mutex.unlock();
					 enter(k);
				 }
				 scheduler.ScheduleAfter(std::chrono::seconds(30),timedHome);
		});
		// Display welcome immediatly
		scheduler.ScheduleAfter(std::chrono::milliseconds(100),timedHome);

	}
	virtual ~HomerMenu() {}

	void addActionVisitor(const shared_ptr<MenuActionVisitor>& visitor) {
		visitors.push_back(visitor);
	}
};


} /* namespace homerio */



#endif /* HOMERMENU_HPP_ */
