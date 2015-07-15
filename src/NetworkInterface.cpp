#include <NetworkInterface.hpp>

NetworkInterface::NetworkInterface() {
	_name = "Unnamed interface";
	_flags = 0;
	_address = _netmask = _broadcast = _pointToPoint = (struct sockaddr *) 0;
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

	deleteIfNotNull(_address);

	if(ifa->ifa_addr != 0) {
		if(_address == 0)
			if(ifa->ifa_addr->sa_family == AF_INET)
				_address = (struct sockaddr *) new struct sockaddr_in;
			else if(ifa->ifa_addr->sa_family == AF_INET6)
				_address = (struct sockaddr *) new struct sockaddr_in6;
		if(ifa->ifa_addr->sa_family == AF_INET)
			::memcpy(_address, ifa->ifa_addr, sizeof(struct sockaddr_in));
		else if(ifa->ifa_addr->sa_family == AF_INET6)
			::memcpy(_address, ifa->ifa_addr, sizeof(struct sockaddr_in6));
	}

	deleteIfNotNull(_netmask);

	if(ifa->ifa_netmask != 0) {
		if(_netmask == 0)
			if(ifa->ifa_netmask->sa_family == AF_INET)
				_netmask = (struct sockaddr *) new struct sockaddr_in;
			else if(ifa->ifa_addr->sa_family == AF_INET6)
				_netmask = (struct sockaddr *) new struct sockaddr_in6;
		if(ifa->ifa_netmask->sa_family == AF_INET)
			::memcpy(_netmask, ifa->ifa_netmask, sizeof(struct sockaddr_in));
		else if(ifa->ifa_addr->sa_family == AF_INET6)
			::memcpy(_netmask, ifa->ifa_netmask, sizeof(struct sockaddr_in6));
	}

	deleteIfNotNull(_broadcast);

	if((_flags & IFF_BROADCAST) and (ifa->ifa_broadaddr != 0)) {
		if(_broadcast == 0)
			if(ifa->ifa_broadaddr->sa_family == AF_INET)
				_broadcast = (struct sockaddr *) new struct sockaddr_in;
			else if(ifa->ifa_addr->sa_family == AF_INET6)
				_broadcast = (struct sockaddr *) new struct sockaddr_in6;
		if(ifa->ifa_broadaddr->sa_family == AF_INET)
			::memcpy(_broadcast, ifa->ifa_broadaddr, sizeof(struct sockaddr_in));
		else if(ifa->ifa_addr->sa_family == AF_INET6)
			::memcpy(_broadcast, ifa->ifa_broadaddr, sizeof(struct sockaddr_in6));

	}

	deleteIfNotNull(_pointToPoint);

	if((_flags & IFF_POINTOPOINT) and (ifa->ifa_dstaddr != 0)) {
		if(_pointToPoint == 0)
			if(ifa->ifa_dstaddr->sa_family == AF_INET)
				_pointToPoint = (struct sockaddr *) new struct sockaddr_in;
			else if(ifa->ifa_addr->sa_family == AF_INET6)
				_pointToPoint = (struct sockaddr *) new struct sockaddr_in6;
		if(ifa->ifa_dstaddr->sa_family == AF_INET)
			::memcpy(_pointToPoint, ifa->ifa_broadaddr, sizeof(struct sockaddr_in));
		else if(ifa->ifa_addr->sa_family == AF_INET6)
			::memcpy(_pointToPoint, ifa->ifa_broadaddr, sizeof(struct sockaddr_in6));
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
	if(_address != 0) {
		if(_address->sa_family == AF_INET) {
			char address[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &((struct sockaddr_in *) _address)->sin_addr, address, INET_ADDRSTRLEN);
			return address;
		}
		else if(_address->sa_family == AF_INET6) {
			char address[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &((struct sockaddr_in6 *) _address)->sin6_addr, address, INET6_ADDRSTRLEN);
			return address;
		}
	}
	return "";
}

std::string NetworkInterface::netmask() const {
	if(_netmask != 0) {
		if(_netmask->sa_family == AF_INET) {
			char address[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &((struct sockaddr_in *) _netmask)->sin_addr, address, INET_ADDRSTRLEN);
			return address;
		}
		else if(_netmask->sa_family == AF_INET6) {
			char address[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &((struct sockaddr_in6 *) _netmask)->sin6_addr, address, INET6_ADDRSTRLEN);
			return address;
		}
	}
	return "";
}

void AAA::allInterfaces() {
	struct ifaddrs *ifap, *ifa;
	if(::getifaddrs(&ifap) == GETIFADDR_ERROR) {
		throw Exception(strerror(errno));
	}
	NetworkInterface networkInterface;
	for(ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		networkInterface.initializeWith(ifa);
		std::cout << networkInterface << std::endl;
	}
	::freeifaddrs(ifap);
}

std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface) {
	ostream << "NetworkInterface(";
	ostream << "name=" << networkInterface.name();
	ostream << ",address=" << networkInterface.address();
	ostream << ",netmask=" << networkInterface.netmask();
	ostream << ")";
	return ostream;
}
