#ifndef __SOCKET_ADDRESS_H__
#define __SOCKET_ADDRESS_H__

#include <iostream>

#include <string.h>

#include <arpa/inet.h>

#include <Exception.hpp>
#include <Assert.hpp>

class SocketAddress {
public:
	SocketAddress(const struct sockaddr *socketAddress);
	SocketAddress(const SocketAddress &socketAddress);
	~SocketAddress();
	std::string address() const;
protected:
	void initializeWith(const struct sockaddr *socketAddress);
protected:
	struct sockaddr *_socketAddress;
};

std::ostream& operator<<(std::ostream &ostream, const SocketAddress &socketAddress);

#endif /* __SOCKET_ADDRESS_H__ */
