#ifndef __IP_H__
#define __IP_H__

#include <arpa/inet.h>
#include <unistd.h>

/*
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|Version|  IHL  |Type of Service|          Total Length         |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|         Identification        |Flags|      Fragment Offset    |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|  Time to Live |    Protocol   |         Header Checksum       |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                       Source Address                          |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                    Destination Address                        |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                    Options                    |    Padding    |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                        Example Internet Datagram Header
                       https://tools.ietf.org/html/rfc791
*/

#define IPV4_VERSION 4
#define IPV4_IHL 5
#define IPV4_PROTO_ICMP 1

struct ipv4_t {
	unsigned char IHL:4, version:4;
	unsigned char typeOfService;
	unsigned short totalLength;
	unsigned short identifier;
	unsigned short fragmentOffset:13, flags:3;
	unsigned char timeToLive;
	unsigned char protocol;
	unsigned short headerChecksum;
	unsigned int sourceAddress;
	unsigned int destinationAddress;

} __attribute__ ((__packed__));

#endif /* __IP_H__ */
