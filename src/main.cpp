/* C++ */
#include <iostream>

/* C */
#include <cstring>
#include <cmath>

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

#define GETIFADDR_ERROR -1
#define GETADDRINFO_SUCCESS 0
#define SOCKET_ERROR -1
#define SETSOCKOPT_ERROR -1
#define SENDTO_ERROR -1
#define RECV_ERROR -1

#define TIMEOUT 1

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

unsigned short internetChecksum(const void *buffer, int count) {
	register long sum = 0;
	unsigned short *addr = (unsigned short *) buffer;
	while(count > 1)  {
		sum += *addr++;
		count -= 2;
	}
	if(count > 0)
		sum += * (unsigned char *) addr;
	while(sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return ~sum;
}

#include <NetworkInterface.hpp>
#include <InternetAddress.hpp>
#include <Socket.hpp>

using namespace std;

class TraceApplication {
public:
	static int execute(int argc, char *argv[]) {

		/* list interfaces */
		vector<NetworkInterface> ifaces = NetworkInterface::allInterfaces();
		for(int i = 0; i < ifaces.size(); i++) {
			cout << ifaces[i] << endl;
		}
		/* resolve hostname */
		vector<InternetAddress> addresses = InternetAddress::resolve("www.google.com");
		for(int i = 0; i < addresses.size(); i++) {
			cout << addresses[i] << endl;
		}
		/* create socket */
		Socket socket;

		return 0;
	}
};

int main(int argc, char *argv[]) {
	return TraceApplication::execute(argc, argv);
}

/*
int main(int argc, char *argv[]) {

	// Create echo-request /
	int echoRequestLength = sizeof(struct ipv4_t) + sizeof(struct icmp_t);
	char *echoRequestBuffer = new char[echoRequestLength];
	::memset(echoRequestBuffer, 0, echoRequestLength);

	struct ipv4_t *ipv4 = (struct ipv4_t *) echoRequestBuffer;

	ipv4->version = IPV4_VERSION;
	ipv4->IHL = IPV4_IHL;
	ipv4->totalLength = ::htons((unsigned short) echoRequestLength & 0xFFFF);
	ipv4->timeToLive = 64;
	ipv4->identifier = ::htons((unsigned short) ::getpid() & 0xFFFF);
	ipv4->protocol = IPV4_PROTO_ICMP;
	ipv4->sourceAddress = source.sin_addr.s_addr;
	ipv4->destinationAddress = destination.sin_addr.s_addr;

	struct icmp_t *icmp = (struct icmp_t *) (echoRequestBuffer + sizeof(struct ipv4_t));

	icmp->type = ICMP_ECHO_REQUEST;
	icmp->data.echoRequest.identifier = ipv4->identifier;
	icmp->data.echoRequest.sequenceNumber = ::htons((unsigned short) 1);
	icmp->checksum = internetChecksum(icmp, sizeof(struct icmp_t));

	// Create receive buffer /
	int length = 1500;
	char *receptionBuffer = new char[length];

	bool done = false;
	int pingPerTtl = 0;

	ipv4->timeToLive = 1;

	while(!(done and (pingPerTtl > (maxPingPerTtl-1)))) {

		// Increment ttl /
		if(pingPerTtl == maxPingPerTtl) {
			ipv4->timeToLive++;
			pingPerTtl = 0;
		}

		// Start time measurement /
		struct timespec start, stop;
		// CLOCK_MONOTONIC
		clock_gettime(CLOCK_MONOTONIC, &start);

		// Send echoRequest /
		if((answer = ::sendto(socketDescriptor, echoRequestBuffer, echoRequestLength, 0, (struct sockaddr *) &destination, sizeof(struct sockaddr_in))) == SENDTO_ERROR) {
			cerr << strerror(errno) << endl;
			return -1;
		}

		// Receive answer /
		answer = ::recv(socketDescriptor, receptionBuffer, length, 0);

		// Stop time measurement /
		clock_gettime(CLOCK_MONOTONIC, &stop);

		timespec temp;
		if ((stop.tv_nsec - start.tv_nsec) < 0) {
			temp.tv_sec = stop.tv_sec - start.tv_sec - 1;
			temp.tv_nsec = 1000000000 + stop.tv_nsec - start.tv_nsec;
		} else {
			temp.tv_sec = stop.tv_sec - start.tv_sec;
			temp.tv_nsec = stop.tv_nsec - start.tv_nsec;
		}

		unsigned int millis = (unsigned int) ::round(((double) temp.tv_nsec) / 1000000.0);
		//long int millis = temp.tv_nsec;


		// Check answer is valid //
		if((answer == RECV_ERROR)) {
			if(errno == EAGAIN) {
				//cout << "|TIMED_OUT" << endl;
				continue;
			}
			else {
				cerr << strerror(errno) << endl;
				return -1;
			}
		}

		// Parse buffer, search for: time-exceeded, echo-reply or echo-request /
		struct ipv4_t *answer_ipv4 = (struct ipv4_t *) receptionBuffer;
		struct icmp_t *answer_icmp = (struct icmp_t *) (receptionBuffer + sizeof(struct ipv4_t));

		if((answer_ipv4->version == IPV4_VERSION) and (answer_ipv4->protocol == IPV4_PROTO_ICMP)) {

			char answer_host[NI_MAXHOST], answer_service[NI_MAXSERV];
			struct sockaddr_in answer_address;
			::memset(&answer_address, 0, sizeof(struct sockaddr_in));
			answer_address.sin_family = AF_INET;
			answer_address.sin_addr.s_addr = answer_ipv4->sourceAddress;
			answer = getnameinfo((struct sockaddr *) &answer_address, sizeof(struct sockaddr_in), answer_host, NI_MAXHOST, answer_service, NI_MAXSERV, NI_NUMERICSERV);
			if(answer != 0) {
				cerr << gai_strerror(answer) << endl;
				answer_host[0] = 0;
			}

			cout << (int) ipv4->timeToLive << "|";

			if(answer_icmp->type == ICMP_ECHO_REQUEST) {
				cout << answer_host << " (" << inet_ntoa(answer_address.sin_addr) << ")|" << millis << endl;
			}
			else if(answer_icmp->type == ICMP_ECHO_REPLY) {
				cout << answer_host << " (" << inet_ntoa(answer_address.sin_addr) << ")|" << millis << endl;
				done = true;
			}
			else if(answer_icmp->type == ICMP_TIME_EXCEEDED) {
				cout << answer_host << " (" << inet_ntoa(answer_address.sin_addr) << ")|" << millis << endl;
			}
			else {
				cerr << "[answer_icmp->type != ICMP_TIME_EXCEEDED, ICMP_ECHO_REPLY, ICMP_ECHO_REQUEST]" << endl;
			}
		}
		else {
			cerr << "[(answer_ipv4->version != IPV4_VERSION) or (answer_ipv4->protocol != IPV4_PROTO_ICMP)]" << endl;
		}

		sleep(1);
		pingPerTtl++;
	}

	// Free resources /
	delete[] echoRequestBuffer;
	delete[] receptionBuffer;
	::close(socketDescriptor);

	return 0;
}
*/
