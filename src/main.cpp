/* C++ */
#include <iostream>
#include <NetworkInterface.hpp>
#include <InternetAddress.hpp>
#include <Socket.hpp>
#include <Packet.hpp>
#include <Clock.hpp>

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
		vector<InternetAddress> addresses = InternetAddress::resolve(argv[1]);
		for(int i = 0; i < addresses.size(); i++) {
			cout << addresses[i] << endl;
		}
		/* create socket */
		Socket socket;

		/* create ip header */
		unsigned short pid = (unsigned short) (getpid() & 0xFFFF);

		Packet answer;
		EchoRequest echoRequest(1);
		echoRequest.source(ifaces[1].address());
		echoRequest.destination(addresses[0].address());
		echoRequest.timeToLive(64);
		cout << echoRequest << endl;
		Clock clock;
		clock.start();
		socket.send(addresses[0].address(), echoRequest);
		socket.receive(answer);
		uint64 elapsed = clock.elapsed();
		cout << "(" << elapsed << "ms)" << endl << answer << endl;

		IPv4 *ipv4 = (IPv4 *) &answer;

		cout << "from:" << ipv4->source().hostname() << " to:" << ipv4->destination().hostname() << endl;

		if(ipv4->protocol() == IPV4_PROTO_ICMP) {
			ICMPv4 *icmp = (ICMPv4 *) ipv4;
			if(icmp->type() == ICMP_ECHO_REPLY)
				cout << "echo-reply" << endl;
			else if(icmp->type() == ICMP_ECHO_REQUEST)
				cout << "echo-request" << endl;
			else if(icmp->type() == ICMP_TIME_EXCEEDED)
				cout << "time-exceeded" << endl;
			else
				cout << "icmp" << endl;
		}

		return 0;
	}
};

int main(int argc, char *argv[]) {
	return TraceApplication::execute(argc, argv);
}
