
#include <Packet.hpp>

Packet::Packet() {
	_buffer = _cursor = (char *) 0;
	_size = 0;
}
