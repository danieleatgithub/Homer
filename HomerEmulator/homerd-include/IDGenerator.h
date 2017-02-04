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

#ifndef IDGENERATOR_H_
#define IDGENERATOR_H_
#include <cstdint>
#include <iostream>
#include <mutex>

namespace commodities {

class IDGenerator {
 public:
  static IDGenerator& get_istance() {
    static IDGenerator istance;
    return istance;
  }
  uint32_t getId() {
    std::unique_lock < std::mutex > lock(mutex);
    return id++;
  }

 private:
  IDGenerator() {
    std::unique_lock < std::mutex > lock(mutex);
    id = 0;
  }
  uint32_t id;
  std::mutex mutex;
};

} /* namespace shd */

#endif /* IDGENERATOR_H_ */
