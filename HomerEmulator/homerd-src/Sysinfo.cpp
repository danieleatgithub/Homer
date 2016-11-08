/*
 * Sysinfo.cpp
 *
 *  Created on: 27/gen/2016
 *      Author: daniele
 */

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <Sysinfo.h>
#include <cstring>
#include <string>

using namespace std;

namespace homerio {


string Sysinfo::get_local_ip(const char *ifname) {
 struct ifaddrs * ifAddrStruct=NULL;
 struct ifaddrs * ifa=NULL;
 void * tmpAddrPtr=NULL;


 ip = "No IP available";

 getifaddrs(&ifAddrStruct);

 for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
     if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
         // is a valid IP4 Address
         tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
         char addressBuffer[INET_ADDRSTRLEN];
         inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
         if(strcmp(ifa->ifa_name,ifname) == 0) ip = string(addressBuffer);
      } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
         // is a valid IP6 Address
         tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
         char addressBuffer[INET6_ADDRSTRLEN];
         inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
         //printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
     }
 }
 if (ifAddrStruct!=NULL)
     freeifaddrs(ifAddrStruct);//remember to free ifAddrStruct
 return(ip);
}

} /* namespace homerio */
