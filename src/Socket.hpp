#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <Exception.hpp>
#include <Packet.hpp>
#include <SocketAddress.hpp>

class Socket {
public:
	Socket();
	Socket(const Socket &socket) = delete;
	~Socket();
	void send(const SocketAddress &dst, const Packet &packet) const;
	void receive(Packet &packet) const;
protected:
	int _descriptor;
};

#define SOCKET_ERROR -1
#define SENDTO_ERROR -1
#define RECV_ERROR -1
#define SETSOCKOPT_ERROR -1
#define TIMEOUT_SECONDS 1

#endif /* __SOCKET_H__ */
