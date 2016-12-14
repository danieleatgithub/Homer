/*******************************************************************************
 * Copyright (C) 2016  Daniele Colombo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/

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
  DisplayPanel() {
  }
  ;
  virtual ~DisplayPanel() {
  }
  ;

  void write_line(Line_e l, string& s) {
  }
  void clear_line(Line_e l = NOLINE);
  void set_on(bool state);
  void reset();

};

}

#endif /* DISPLAYPANEL_HPP_ */
