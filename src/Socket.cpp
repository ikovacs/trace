
#include <Socket.hpp>

Socket::Socket() {
	int hdrincl = 1;
	struct timeval timeOut;
	timeOut.tv_sec = TIMEOUT_SECONDS;
	timeOut.tv_usec = 0;
	if((_descriptor = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == SOCKET_ERROR)
		throw FullException(strerror(errno));
	if(::setsockopt(_descriptor, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) == SETSOCKOPT_ERROR)
		throw FullException(strerror(errno));
	if(::setsockopt(_descriptor, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(struct timeval)) == SETSOCKOPT_ERROR)
		throw FullException(strerror(errno));
	if(::setsockopt(_descriptor, SOL_SOCKET, SO_SNDTIMEO, &timeOut, sizeof(struct timeval)) == SETSOCKOPT_ERROR)
		throw FullException(strerror(errno));
}

Socket::~Socket() {
	close(_descriptor);
}

void Socket::send(const SocketAddress &dst, const Packet &packet) const {
	//TODO: send packet
}
