#ifndef __INTERNET_ADDRESS_H__
#define __INTERNET_ADDRESS_H__

#include <iostream>
#include <vector>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <SocketAddress.hpp>
#include <Exception.hpp>
#include <Assert.hpp>

enum Protocol {  };
enum SocketType { Raw = SOCK_RAW, Datagram = SOCK_DGRAM, Stream = SOCK_STREAM };

class InternetAddress {
public:
	InternetAddress(const char *cannonicalName, const struct addrinfo *ai);
	InternetAddress(const InternetAddress &internetAddress);
	~InternetAddress();
	SocketType socketType() const;
	const SocketAddress& address() const;
	std::string cannonicalName() const;
	static std::vector<InternetAddress> resolve(const char *node);
protected:
	int _flags;
	AddressFamily _family;
	SocketType _socketType;
	int _protocol;
	SocketAddress *_address;
	std::string _canonicalName;
};

std::ostream& operator<<(std::ostream &ostream, const InternetAddress &internetAddress);

#define GETADDRINFO_SUCCESS 0
#define ServiceNone 0

#endif /* __INTERNET_ADDRESS_H__ */
