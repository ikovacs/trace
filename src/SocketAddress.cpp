
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
	AssertNotNull(socketAddress);
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
const struct sockaddr* SocketAddress::sockaddr() const {
	return _socketAddress;
}
AddressFamily SocketAddress::family() const {
	return (AddressFamily) _socketAddress->sa_family;
}
std::string SocketAddress::toString() const {
	AssertNotNull(_socketAddress);
	int max = (INET_ADDRSTRLEN > INET_ADDRSTRLEN)? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
	char stringAddress[max];
	if(_socketAddress->sa_family == AF_INET)
		inet_ntop(AF_INET, &((struct sockaddr_in *) _socketAddress)->sin_addr, stringAddress, max);
	else
		inet_ntop(AF_INET6, &((struct sockaddr_in6 *) _socketAddress)->sin6_addr, stringAddress, max);
	return stringAddress;
}
std::string SocketAddress::hostname() const {
	char host[NI_MAXHOST], service[NI_MAXSERV];
	int size = (_socketAddress->sa_family == AF_INET)? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
	int answer = getnameinfo(_socketAddress, size, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
	if(answer != GETNAMEINFO_SUCCESS)
		throw FullException(gai_strerror(answer));
	return std::string(host);
}
std::ostream& operator<<(std::ostream &ostream, const SocketAddress &socketAddress) {
	ostream << socketAddress.toString();
	return ostream;
}
