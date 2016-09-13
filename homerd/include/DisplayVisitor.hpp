/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef DISPLAYVISITOR_HPP_
#define DISPLAYVISITOR_HPP_

#include <Menu.hpp>
#include <MenuVisitors.hpp>
#include <Display.h>

#define TRACE_TMP do { \
	cout << __PRETTY_FUNCTION__ << l.get_label() << "" << endl; \
} while(0)

#define TRACE_TMP2 do { \
	cout << __PRETTY_FUNCTION__ << l.get_label() << ">" << endl; \
} while(0)

namespace homerio {

class DisplayVisitor  : public MenuActionVisitor {
	Display& dp;
public:
	DisplayVisitor(Display& display) : dp(display) {	};
	virtual ~DisplayVisitor() {};

	virtual void enter(MenuLeaf&  l,KeyButton& k) 	{
		dp.dpy_puts(l.get_label().c_str());
		dp.line2_home();
		dp.dpy_puts(l.getValue().c_str());
		TRACE_TMP;
	}
	virtual void leave(MenuLeaf&  l,KeyButton& k) {
		dp.clear();
		TRACE_TMP;
	}
	virtual void click(MenuLeaf& l,KeyButton& k) {
		TRACE_TMP;
	}


	virtual void enter(SubMenu& l,KeyButton& k) 	{
		dp.dpy_puts(l.get_label().c_str());
		dp.dpy_puts(">");
		TRACE_TMP2;
	}
	virtual void leave(SubMenu& l,KeyButton& k) {
		dp.clear();
		TRACE_TMP2;
	}
	virtual void click(SubMenu& l,KeyButton& k) {
		TRACE_TMP2;
	}

};

}


#endif /* DISPLAYVISITOR_HPP_ */
