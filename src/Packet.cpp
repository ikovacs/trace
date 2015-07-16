
#include <Packet.hpp>

std::ostream& operator<<(std::ostream& ostream, const Packet &packet) {
	ostream << "Packet("<< packet._size << " bytes) | ";
	for(int i = 0; (packet._size > 0) and (i < (packet._size - 1)); i++) {
		ostream << std::hex << (int) (unsigned char) packet._packet[i] << " ";
	}
	if(packet._size > 0)
		ostream << std::hex << (int) (unsigned char) packet._packet[(packet._size - 1)];
	return ostream;
}

EchoRequest::EchoRequest() {
	int identifier = htons((unsigned short) (getpid() & 0xFFFF));
	_size = sizeof(struct ipv4_t) + sizeof(struct icmp_t);
	_packet = new char[_size];
	memset(_packet, 0, _size);
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));

	ipv4->version = IPV4_VERSION;
	ipv4->IHL = IPV4_IHL;
	ipv4->totalLength = htons((unsigned short) (_size & 0xFFFF));
	ipv4->identifier = identifier;
	ipv4->timeToLive = 255;
	ipv4->protocol = IPV4_PROTO_ICMP;

	icmp->type = ICMP_ECHO_REQUEST;
	icmp->code = 0;
	icmp->data.echoRequest.identifier = identifier;
	icmp->data.echoRequest.sequenceNumber = htons((unsigned short) 1);
	icmp->checksum = internetChecksum(icmp, sizeof(struct icmp_t));
}

void EchoRequest::source(const SocketAddress &source) {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	ipv4->sourceAddress = ((const struct sockaddr_in *) source.sockaddr())->sin_addr.s_addr;
}

void EchoRequest::destination(const SocketAddress &destination) {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	ipv4->destinationAddress = ((const struct sockaddr_in *) destination.sockaddr())->sin_addr.s_addr;
}

EchoRequest::~EchoRequest() {
	delete[] _packet;
}
