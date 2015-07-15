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
	NetworkInterface();
	~NetworkInterface();

	void initializeWith(const struct ifaddrs *ifa);

	bool isUp() const;
	bool isRunning() const;
	bool isPointToPointLink() const;

	std::string name() const;
	std::string address() const;
	std::string netmask() const;

protected:
	std::string _name;
	unsigned int _flags;
	struct sockaddr *_address;
	struct sockaddr *_netmask;
	struct sockaddr *_broadcast;
	struct sockaddr *_pointToPoint;
};

std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface);

class AAA {
public:
	void allInterfaces();
};

#define GETIFADDR_ERROR -1
#define deleteIfNotNull(pointer) if(pointer != 0) { delete pointer; pointer = 0; }

#endif /* __NETWORK_INTERFACE_H__ */
