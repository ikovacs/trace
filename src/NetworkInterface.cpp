#include <NetworkInterface.hpp>

NetworkInterface::NetworkInterface(const struct ifaddrs *ifa) {
	this->initializePointers();
	this->initializeWith(ifa);
}

NetworkInterface::NetworkInterface(const NetworkInterface &networkInterface) {
	this->initializePointers();
	_name = networkInterface._name;
	_flags = networkInterface._flags;
	if(networkInterface._address != 0) {
		_address = new SocketAddress(*networkInterface._address);
	}
	if(networkInterface._netmask != 0) {
		_netmask = new SocketAddress(*networkInterface._netmask);
	}
	if(networkInterface._broadcast != 0) {
		_broadcast = new SocketAddress(*networkInterface._broadcast);
	}
	if(networkInterface._pointToPoint != 0) {
		_pointToPoint = new SocketAddress(*networkInterface._pointToPoint);
	}
}

NetworkInterface::~NetworkInterface() {
	deleteIfNotNull(_address);
	deleteIfNotNull(_netmask);
	deleteIfNotNull(_broadcast);
	deleteIfNotNull(_pointToPoint);
}

void NetworkInterface::initializePointers() {
	_address = _netmask = _broadcast = _pointToPoint = (SocketAddress *) 0;
}

/* TODO: quitar código repetido de load(), optimizar pedido de memoria. */

void NetworkInterface::initializeWith(const struct ifaddrs *ifa) {
	AssertNotNull(ifa);
	if(ifa->ifa_name != 0)
		_name = ifa->ifa_name;
	else
		_name = "Unnamed interface";

	_flags = ifa->ifa_flags;

	deleteIfNotNull(_address);
	if(ifa->ifa_addr != 0) {
		_address = new SocketAddress(ifa->ifa_addr);
	}
	deleteIfNotNull(_netmask);
	if(ifa->ifa_netmask != 0) {
		_netmask = new SocketAddress(ifa->ifa_netmask);
	}
	deleteIfNotNull(_broadcast);
	if((_flags & IFF_BROADCAST) and (ifa->ifa_broadaddr != 0)) {
		_broadcast = new SocketAddress(ifa->ifa_broadaddr);
	}
	deleteIfNotNull(_pointToPoint);
	if((_flags & IFF_POINTOPOINT) and (ifa->ifa_dstaddr != 0)) {
		_pointToPoint = new SocketAddress(ifa->ifa_dstaddr);
	}
}

bool NetworkInterface::isUp() const {
	/* Interface is running. */
	return _flags & IFF_UP;
}

bool NetworkInterface::isRunning() const {
	/* Resources allocated. */
	return _flags & IFF_RUNNING;
}

bool NetworkInterface::isPointToPointLink() const {
	/* Interface is a point-to-point link. */
	return _flags & IFF_POINTOPOINT;
}

std::string NetworkInterface::name() const {
	return _name;
}

const SocketAddress& NetworkInterface::address() const {
	AssertNotNull(_address);
	return *_address;
}

const SocketAddress& NetworkInterface::netmask() const {
	AssertNotNull(_netmask);
	return *_netmask;
}

/*
	Description: Yields a vector with all network interfaces for running host.
*/

std::vector<NetworkInterface> NetworkInterface::allInterfaces() {
	struct ifaddrs *ifap, *ifa;
	if(::getifaddrs(&ifap) == GETIFADDR_ERROR)
		throw Exception(strerror(errno));
	std::vector<NetworkInterface> interfaces;
	for(ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		if(ifa->ifa_addr->sa_family != AF_PACKET) {
			NetworkInterface interface(ifa);
			interfaces.push_back(interface);
		}
	}
	::freeifaddrs(ifap);
	return interfaces;
}

std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface) {
	ostream << "NetworkInterface(";
	ostream << "name=" << networkInterface.name();
	ostream << ",address=" << networkInterface.address();
	ostream << ",netmask=" << networkInterface.netmask();
	ostream << ")";
	return ostream;
}
