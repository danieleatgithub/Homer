/*
 * MenuComponentVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MENUVISITORS_HPP_
#define MENUVISITORS_HPP_

#include <Menu.hpp>
#include <KeyPanel.hpp>



namespace homerio {

class MenuComponent;
class MenuLeaf;
class SubMenu;

class MenuActionVisitor
{
public:
	virtual ~MenuActionVisitor() {};
	virtual void enter(MenuLeaf& l,KeyButton& k) = 0;
	virtual void leave(MenuLeaf& l,KeyButton& k) = 0;
	virtual void click(MenuLeaf& l,KeyButton& k) = 0;

	virtual void enter(SubMenu& l,KeyButton& k) 	= 0;
	virtual void leave(SubMenu& l,KeyButton& k) = 0;
	virtual void click(SubMenu& l,KeyButton& k) = 0;

};

class MenuNavigatorVisitor
{
public:
	virtual ~MenuNavigatorVisitor() {};
	virtual shared_ptr<MenuComponent> move(MenuLeaf& m,KeyButton& k) = 0;
	virtual shared_ptr<MenuComponent> move(SubMenu& m,KeyButton& k) = 0;

};

}

#endif /* MENUVISITORS_HPP_ */
