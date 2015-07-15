#include <NetworkInterface.hpp>

void AAA::allInterfaces() {
	struct ifaddrs *ifap, *ifa;
	if(::getifaddrs(&ifap) == GETIFADDR_ERROR) {
		throw Exception(strerror(errno));
	}
	for(ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		NetworkInterface networkInterface;
		networkInterface.load(ifa);
		std::cout << networkInterface << std::endl;
	}
	::freeifaddrs(ifap);
}

std::ostream& operator<<(std::ostream &ostream, const NetworkInterface &networkInterface) {
	return ostream;
}
