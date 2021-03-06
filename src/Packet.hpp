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
	SocketAddress source() const;
	SocketAddress destination() const;
	void timeToLive(unsigned char ttl);
	int protocol() const;
};

class ICMPv4 : public IPv4 {
public:
	ICMPv4(unsigned char type);
	~ICMPv4();
	int type() const;
};

class Echo : public ICMPv4 {
public:
	Echo(unsigned char type);
	int identifier() const;
	void sequenceNumber(unsigned short sequenceNumber);
	int sequenceNumber() const;
};

class EchoRequest : public Echo {
public:
	EchoRequest();
	~EchoRequest();
};

class EchoReply : public ICMPv4 {};

class TimeExceeded : public ICMPv4 {};

class DestinationUnreachable : public ICMPv4 {};

std::ostream& operator<<(std::ostream& ostream, const Packet &packet);

#define PACKET_MAX 65535

#endif /* __PACKET_H__ */
