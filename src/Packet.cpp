
#include <Packet.hpp>

//TODO: Probar que no pierda memoria en un programa aparte.

Packet::Packet() {
	/* Packet */
	_size = sizeof(struct ipv4_t) + sizeof(struct icmp_t);
	_packet = new char[PACKET_MAX];
	memset(_packet, 0, PACKET_MAX);
}
Packet::~Packet() {
	delete[] _packet;
}

IPv4::IPv4(unsigned char protocol) : Packet() {
	/* IPv4 */
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	ipv4->version = IPV4_VERSION;
	ipv4->IHL = IPV4_IHL;
	ipv4->totalLength = htons((unsigned short) (_size & 0xFFFF));
	ipv4->identifier = htons((unsigned short) (getpid() & 0xFFFF));
	ipv4->timeToLive = 255;
	ipv4->protocol = protocol;
}
IPv4::~IPv4() {}
void IPv4::source(const SocketAddress &source) {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	ipv4->sourceAddress = ((const struct sockaddr_in *) source.sockaddr())->sin_addr.s_addr;
}
void IPv4::destination(const SocketAddress &destination) {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	ipv4->destinationAddress = ((const struct sockaddr_in *) destination.sockaddr())->sin_addr.s_addr;
}
SocketAddress IPv4::source() const {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	struct sockaddr_in address;
	memset(&address, 0, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ipv4->sourceAddress;
	return SocketAddress((const struct sockaddr *) &address);
}
SocketAddress IPv4::destination() const {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	struct sockaddr_in address;
	memset(&address, 0, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ipv4->destinationAddress;
	return SocketAddress((const struct sockaddr *) &address);
}
void IPv4::timeToLive(unsigned char ttl) {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	ipv4->timeToLive = ttl;
}
int IPv4::protocol() const {
	ipv4_t *ipv4 = (ipv4_t *) _packet;
	return ipv4->protocol;
}

ICMPv4::ICMPv4(unsigned char type) : IPv4(IPV4_PROTO_ICMP) {
	/* ICMPv4 */
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));
	icmp->type = type;
	icmp->code = 0;
}
ICMPv4::~ICMPv4() {}
int ICMPv4::type() const {
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));
	return icmp->type;
}

Echo::Echo(unsigned char type) : ICMPv4(type) {
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));
	icmp->data.echoRequest.identifier = htons((unsigned short) (getpid() & 0xFFFF));
	icmp->data.echoRequest.sequenceNumber = htons((unsigned short) 0);
	icmp->checksum = internetChecksum(icmp, sizeof(struct icmp_t));
}
int Echo::identifier() const {
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));
	return (int) ntohs(icmp->data.echoRequest.identifier);
}
void Echo::sequenceNumber(unsigned short sequenceNumber) {
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));
	icmp->data.echoRequest.sequenceNumber = htons(sequenceNumber);
	icmp->checksum = 0;
	icmp->checksum = internetChecksum(icmp, sizeof(struct icmp_t));
}
int Echo::sequenceNumber() const {
	icmp_t *icmp = (icmp_t *) (_packet + sizeof(struct ipv4_t));
	return ntohs(icmp->data.echoRequest.sequenceNumber);
}

EchoRequest::EchoRequest() : Echo(ICMP_ECHO_REQUEST) {}
EchoRequest::~EchoRequest() {}

std::ostream& operator<<(std::ostream& ostream, const Packet &packet) {
	std::ios state(0);
    state.copyfmt(ostream);
	ostream << "Packet("<< packet._size << " bytes) [";
	for(int i = 0; (packet._size > 0) and (i < (packet._size - 1)); i++) {
		ostream << std::hex << std::setw(2) << std::setfill('0') << (int) (unsigned char) packet._packet[i] << " ";
	}
	if(packet._size > 0)
		ostream << std::hex << std::setw(2) << std::setfill('0') << (int) (unsigned char) packet._packet[(packet._size - 1)];
	ostream << "]";
	ostream.copyfmt(state);
	return ostream;
}
