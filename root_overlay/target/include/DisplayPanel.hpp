/*
 * DisplayPanel.hpp
 *
 *  Created on: 20/ago/2016
 *      Author: daniele
 */

#ifndef DISPLAYPANEL_HPP_
#define DISPLAYPANEL_HPP_


#include <string>
using namespace std;

namespace homerio {

enum Line_e {
	NOLINE = 0,
	LINE_1 = 1,
	LINE_2 = 2
};

class DisplayPanel {
public:
	DisplayPanel() {};
	virtual ~DisplayPanel() {};

	void write_line(Line_e l, string& s) {}
	void clear_line(Line_e l = NOLINE);
	void set_on(bool state);
	void reset();


};

}


#endif /* DISPLAYPANEL_HPP_ */
