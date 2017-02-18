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

#ifndef SYSINFO_H_
#define SYSINFO_H_
#include <string>

using namespace std;

namespace commodities {

class Sysinfo {
 private:
  Sysinfo() {
  }
  ;
  string ip;

 public:

  static Sysinfo& get_instance() {
    static Sysinfo instance;
    return instance;
  }

  // FIXME: protect with mutex
  string get_local_ip(const char *ifname);
};

} /* namespace homerio */

#endif /* SYSINFO_H_ */
