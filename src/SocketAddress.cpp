
#include <SocketAddress.hpp>

SocketAddress::SocketAddress(const struct sockaddr *socketAddress) {
	this->initializeWith(socketAddress);
}
SocketAddress::SocketAddress(const SocketAddress &socketAddress) {
	this->initializeWith(socketAddress._socketAddress);
}
SocketAddress::~SocketAddress() {
	delete _socketAddress;
}
void SocketAddress::initializeWith(const struct sockaddr *socketAddress) {
	Assert::notNull(socketAddress, __FUNCTION__, __FILE__, __LINE__);
	_socketAddress = 0;
	if(socketAddress->sa_family == AF_INET) { /* IPv4 */
		_socketAddress = (struct sockaddr *) new struct sockaddr_in;
		::memcpy(_socketAddress, socketAddress, sizeof(struct sockaddr_in));
	}
	else { /* IPv6 */
		_socketAddress = (struct sockaddr *) new struct sockaddr_in6;
		::memcpy(_socketAddress, socketAddress, sizeof(struct sockaddr_in6));
	}
}
std::string SocketAddress::address() const {
	Assert::notNull(_socketAddress, __FUNCTION__, __FILE__, __LINE__);
	if(_socketAddress->sa_family == AF_INET) {
		char address[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &((struct sockaddr_in *) _socketAddress)->sin_addr, address, INET_ADDRSTRLEN);
		return address;
	}
	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &((struct sockaddr_in6 *) _socketAddress)->sin6_addr, address, INET6_ADDRSTRLEN);
	return address;
}
std::ostream& operator<<(std::ostream &ostream, const SocketAddress &socketAddress) {
	ostream << socketAddress.address();
	return ostream;
}
