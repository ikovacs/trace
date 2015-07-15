#include <NetworkInterface.hpp>

NetworkInterface::NetworkInterface() {
	_name = "Unnamed interface";
	_flags = 0;
	_address = _netmask = _broadcast = _pointToPoint = (SocketAddress *) 0;
}

NetworkInterface::NetworkInterface(const struct ifaddrs *ifa) : NetworkInterface() {
	this->initializeWith(ifa);
}

NetworkInterface::NetworkInterface(const NetworkInterface &networkInterface) : NetworkInterface() {

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

/* TODO: quitar código repetido de load(), optimizar pedido de memoria. */

void NetworkInterface::initializeWith(const struct ifaddrs *ifa) {

	_name = ifa->ifa_name;
	_flags = ifa->ifa_flags;

	if(ifa->ifa_addr->sa_family != AF_PACKET) {

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

std::string NetworkInterface::address() const {
	return (_address != 0)? _address->address() : "";
}

std::string NetworkInterface::netmask() const {
	return (_netmask != 0)? _netmask->address() : "";
}

/*
	Description: Yields a list with all network interfaces for running host.
*/

std::list<NetworkInterface> NetworkInterface::allInterfaces() {
	struct ifaddrs *ifap, *ifa;
	if(::getifaddrs(&ifap) == GETIFADDR_ERROR)
		throw Exception(strerror(errno));
	std::list<NetworkInterface> interfaces;
	for(ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		NetworkInterface interface(ifa);
		interfaces.push_back(interface);
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
