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

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <Sysinfo.h>
#include <cstring>
#include <string>

using namespace std;

namespace commodities {

string Sysinfo::get_local_ip(const char *ifname) {
  struct ifaddrs * ifAddrStruct = NULL;
  struct ifaddrs * ifa = NULL;
  void * tmpAddrPtr = NULL;

  ip = string("No IP available");

  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr->sa_family == AF_INET) {  // check it is IP4
      // is a valid IP4 Address
      tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
      char addressBuffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      if (strcmp(ifa->ifa_name, ifname) == 0)
        ip = string(addressBuffer);
    } else if (ifa->ifa_addr->sa_family == AF_INET6) {  // check it is IP6
      // is a valid IP6 Address
      tmpAddrPtr = &((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr;
      char addressBuffer[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
      //printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
    }
  }
  if (ifAddrStruct != NULL)
    freeifaddrs(ifAddrStruct);         //remember to free ifAddrStruct

  return (ip);
}

} /* namespace commodities */
