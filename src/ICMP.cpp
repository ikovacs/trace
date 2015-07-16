
#include <ICMP.hpp>

ICMP::ICMP(unsigned char type) {
	_header = new struct icmp_t;
	::memset(_header, 0, sizeof(struct icmp_t));
	_header->type = (unsigned char) ((int) type & 0xFF);
	_header->checksum = internetChecksum(_header, sizeof(struct icmp_t));
}

ICMP::~ICMP() {
	delete _header;
}

EchoRequest::EchoRequest(unsigned short identifier, unsigned short sequenceNumber) : ICMP(ICMP_ECHO_REQUEST) {
	_header->data.echoRequest.identifier = htons(identifier);
	_header->data.echoRequest.sequenceNumber = htons(sequenceNumber);
	_header->checksum = 0;
	_header->checksum = internetChecksum(_header, sizeof(struct icmp_t));
}

unsigned short internetChecksum(const void *buffer, int count) {
	register long sum = 0;
	unsigned short *addr = (unsigned short *) buffer;
	while(count > 1)  {
		sum += *addr++;
		count -= 2;
	}
	if(count > 0)
		sum += * (unsigned char *) addr;
	while(sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return ~sum;
}
