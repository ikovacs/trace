#ifndef __NETWORK_INTERFACE_H__
#define __NETWORK_INTERFACE_H__

#include <iostream>
#include <vector>

#include <sys/types.h>
#include <ifaddrs.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include <string.h>

#include <arpa/inet.h>

#include <Exception.hpp>
#include <SocketAddress.hpp>

class NetworkInterface {
public:
	NetworkInterface(const struct ifaddrs *ifa);
	NetworkInterface(const NetworkInterface &networkInterface);
	~NetworkInterface();

	bool isUp() const;
	bool isRunning() const;
	bool isPointToPointLink() const;

	std::string name() const;
	const SocketAddress& address() const;
	const SocketAddress& netmask() const;

	static std::vector<NetworkInterface> allInterfaces();

protected:
	void initializePointers();
	void initializeWith(const struct ifaddrs *ifa);

protected:
	std::string _name;
	unsigned int _flags;
	SocketAddress *_address;
	SocketAddress *_netmask;
	SocketAddress *_broadcast;
	SocketAddress *_pointToPoint;
};

std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface);

#define GETIFADDR_ERROR -1
#define deleteIfNotNull(pointer) if(pointer != 0) { delete pointer; pointer = 0; }

#endif /* __NETWORK_INTERFACE_H__ */
