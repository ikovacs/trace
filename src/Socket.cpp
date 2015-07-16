
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
	int answer = ::sendto(_descriptor, packet._packet, packet._size, 0, dst.sockaddr(), sizeof(struct sockaddr_in));
	if(answer == SENDTO_ERROR)
		throw FullException(strerror(errno));
}

void Socket::receive(Packet &packet) const {
	int answer = ::recv(_descriptor, packet._packet, packet._size, 0);
	if(answer == RECV_ERROR)
		throw FullException(strerror(errno));
}
