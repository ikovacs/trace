#ifndef __PACKET_H__
#define __PACKET_H__

#include <IP.hpp>
#include <ICMP.hpp>

class Packet {
public:
	Packet();
	Packet& operator<<(const IP &ip);
	Packet& operator<<(const ICMP &icmp);
	friend class IP;
	friend class ICMP;
protected:
	char *_buffer;
	char *_cursor;
	int _size;
};

#endif /* __PACKET_H__ */
