
#include <IP.hpp>

IP::IP(const SocketAddress &src, const SocketAddress &dst, unsigned short identifier, unsigned char ttl) {
	_header = new struct ipv4_t;
	::memset(_header, 0, sizeof(struct ipv4_t));
	_header->version = IPV4_VERSION;
	_header->IHL = IPV4_IHL;
	_header->identifier = htons(identifier);
	_header->timeToLive = ttl;
	/* TODO: crear subclases de SocketAddress para c/tipo de dirección (¿?) */
	_header->sourceAddress = ((const struct sockaddr_in *) src.sockaddr())->sin_addr.s_addr;
	_header->destinationAddress = ((const struct sockaddr_in *) dst.sockaddr())->sin_addr.s_addr;
}

IP::~IP() {
	delete _header;
}

Packet IP::operator/(const ICMP &icmp) {
	return Packet();
}
