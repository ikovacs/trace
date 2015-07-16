
#include <Packet.hpp>

Packet::Packet() {
	_buffer = _cursor = (char *) 0;
	_size = 0;
}

Packet& Packet::operator<<(const IP &ip) {
}

Packet& Packet::operator<<(const ICMP &icmp) {
}
