#ifndef __PACKET_H__
#define __PACKET_H__

#include <IP.hpp>
#include <ICMP.hpp>
#include <SocketAddress.hpp>

#include <iostream>
#include <iomanip>

class ByteArray {};

class Packet : public ByteArray {
public:
	Packet();
	~Packet();
protected:
	char *_packet;
	int _size;
friend std::ostream& operator<<(std::ostream& ostream, const Packet &packet);
friend class Socket;
};

class IPv6 : public Packet {};

class IPv4 : public Packet {
public:
	IPv4(unsigned char protocol);
	~IPv4();
	void source(const SocketAddress &source);
	void destination(const SocketAddress &destination);
	void timeToLive(unsigned char ttl);
};

class ICMPv4 : public IPv4 {
public:
	ICMPv4(unsigned char type);
	~ICMPv4();
};

class EchoRequest : public ICMPv4 {
public:
	EchoRequest(unsigned short sequenceNumber);
	~EchoRequest();
};

class EchoReply : public ICMPv4 {};

class TimeExceeded : public ICMPv4 {};

class DestinationUnreachable : public ICMPv4 {};

std::ostream& operator<<(std::ostream& ostream, const Packet &packet);

#define PACKET_MAX 65535

#endif /* __PACKET_H__ */
