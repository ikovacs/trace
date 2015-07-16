#ifndef __PACKET_H__
#define __PACKET_H__

class Packet {
public:
	Packet();
	friend class IP;
	friend class ICMP;
protected:
	char *_buffer;
	char *_cursor;
	int _size;
};

#endif /* __PACKET_H__ */
