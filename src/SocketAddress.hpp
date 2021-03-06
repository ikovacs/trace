#ifndef __SOCKET_ADDRESS_H__
#define __SOCKET_ADDRESS_H__

#include <iostream>

#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <Exception.hpp>
#include <Assert.hpp>

enum AddressFamily { IPv4AddressFamily = AF_INET, IPv6AddressFamily = AF_INET6 };

class SocketAddress {
public:
	SocketAddress(const struct sockaddr *socketAddress);
	SocketAddress(const SocketAddress &socketAddress);
	~SocketAddress();
	AddressFamily family() const;
	std::string toString() const;
	const struct sockaddr* sockaddr() const;
	std::string hostname() const;
protected:
	void initializeWith(const struct sockaddr *socketAddress);
protected:
	struct sockaddr *_socketAddress;
};

std::ostream& operator<<(std::ostream &ostream, const SocketAddress &socketAddress);

#define GETNAMEINFO_SUCCESS 0

#endif /* __SOCKET_ADDRESS_H__ */
