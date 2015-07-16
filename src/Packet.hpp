#ifndef __PACKET_H__
#define __PACKET_H__

#include <IP.hpp>
#include <ICMP.hpp>
#include <SocketAddress.hpp>

#include <iostream>
#include <iomanip>

class ByteArray {
};
class Packet : public ByteArray {
protected:
	char *_packet;
	int _size;
friend std::ostream& operator<<(std::ostream& ostream, const Packet &packet);
friend class Socket;
};
std::ostream& operator<<(std::ostream& ostream, const Packet &packet);
class IPv6 : public Packet {};
class IPv4 : public Packet {};
class ICMPv4 : public IPv4 {};
class EchoRequest : public ICMPv4 {
public:
	EchoRequest();
	~EchoRequest();
	void source(const SocketAddress &source);
	void destination(const SocketAddress &destination);
};
class EchoReply : public ICMPv4 {};
class TimeExceeded : public ICMPv4 {};
class DestinationUnreachable : public ICMPv4 {};

#endif /* __PACKET_H__ */
