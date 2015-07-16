#ifndef __ICMP_H__
#define __ICMP_H__

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY 0
#define ICMP_TIME_EXCEEDED 11

/*
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |     Code      |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                                                               |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|      Internet Header + 64 bits of Original Data Datagram      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                      https://tools.ietf.org/html/rfc792
*/

struct icmp_echo_t {
	unsigned short identifier;
	unsigned short sequenceNumber;

} __attribute__ ((__packed__));

struct icmp_t {
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	union {
		icmp_echo_t echoRequest;
		icmp_echo_t echoReply;
		icmp_echo_t timeExceeded;
	} data;

} __attribute__ ((__packed__));

unsigned short internetChecksum(const void *buffer, int count);

#endif /* __ICMP_H__ */
