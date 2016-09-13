/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MOVEVISITOR_HPP_
#define MOVEVISITOR_HPP_
#include <Menu.hpp>
#include <MenuVisitors.hpp>
#include <stack>

namespace homerio {


class MoveVisitor : public MenuNavigatorVisitor {
	stack<reference_wrapper<SubMenu>> history;

public:
	MoveVisitor(shared_ptr<SubMenu> m)  {
		history.push(*m.get());
	};
	virtual ~MoveVisitor() {};
	/**
	 * Reset history and jump back to home
	 * @return
	 * pointer to the first element in root menu
	 */
	shared_ptr<MenuComponent> home() {
		while(history.size() > 1) {
			 history.top().get().home();
			 history.pop();
		}
		history.top().get().home();
		return(history.top().get().get_active_element());
	}


	virtual shared_ptr<MenuComponent> move(MenuLeaf& m,KeyButton& k) 	{
		 switch(k.get_key()) {
			 case BUTTON_UP:
				 return(history.top().get().get_previous_element());
			 case BUTTON_DOWN:
				 return(history.top().get().get_next_element());
			 case BUTTON_LEFT:
				 if(history.size() > 1) {
					 history.top().get().home();
					 history.pop();
				 }
				 return(history.top().get().get_active_element());
			 case BUTTON_RIGHT:
				 return(history.top().get().get_active_element());
			 default:
					throw std::runtime_error((string("Unexpected key ")+ string(k)).c_str());
				 break;
		 }
	}

	virtual shared_ptr<MenuComponent> move(SubMenu& m,KeyButton& k) 	{
		 switch(k.get_key()) {
			 case BUTTON_UP:
				 return(history.top().get().get_previous_element());
			 case BUTTON_DOWN:
				 return(history.top().get().get_next_element());
			 case BUTTON_LEFT:
				 if(history.size() > 1) {
					 history.top().get().home();
					 history.pop();
				 }
				 return(history.top().get().get_active_element());
			 case BUTTON_RIGHT:
				 history.push(m);
				 return(history.top().get().get_active_element());
			 default:
					throw std::runtime_error((string("Unexpected key ")+ string(k)).c_str());
				 break;
		 }
	}
};

}


#endif /* MOVEVISITOR_HPP_ */
