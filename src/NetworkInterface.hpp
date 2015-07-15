#ifndef __NETWORK_INTERFACE_H__
#define __NETWORK_INTERFACE_H__

#include <iostream>

#include <sys/types.h>
#include <ifaddrs.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include <string.h>

#include <arpa/inet.h>

#include <Exception.hpp>

class NetworkInterface {
public:
	void load(const struct ifaddrs *interfaceAddress);
	friend std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface);
};

std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface);

class AAA {
public:
	void allInterfaces();
};

#define GETIFADDR_ERROR -1

#endif /* __NETWORK_INTERFACE_H__ */
