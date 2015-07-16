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
	IPv4();
	~IPv4();
	void source(const SocketAddress &source);
	void destination(const SocketAddress &destination);
};

class ICMPv4 : public IPv4 {
public:
	ICMPv4();
	~ICMPv4();
};

class EchoRequest : public ICMPv4 {
public:
	EchoRequest();
	~EchoRequest();
};

class EchoReply : public ICMPv4 {};

class TimeExceeded : public ICMPv4 {};

class DestinationUnreachable : public ICMPv4 {};

std::ostream& operator<<(std::ostream& ostream, const Packet &packet);

#endif /* __PACKET_H__ */
