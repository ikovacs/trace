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

		/* resolve hostname */
		vector<InternetAddress> addresses = InternetAddress::resolve(argv[1]);

		/* create socket */
		Socket socket;

		/* create echoRequest */
		EchoRequest echoRequest(1);
		echoRequest.source(ifaces[1].address());
		echoRequest.destination(addresses[0].address());
		echoRequest.timeToLive(64);

		Packet answer;
		Clock clock;

		bool done = false;
		unsigned char ttl = 1;
		uint64 elapsed;

		while((!done) and (ttl < 256)) {

			echoRequest.timeToLive(ttl++);

			try {
				clock.start();
				socket.send(addresses[0].address(), echoRequest);
				socket.receive(answer);
				elapsed = clock.elapsed();
			}
			catch(Exception &exception) {
				cerr << exception << endl;
				continue;
			}

			IPv4 *ipv4 = (IPv4 *) &answer;

			if(ipv4->protocol() == IPV4_PROTO_ICMP) {

				ICMPv4 *icmp = (ICMPv4 *) ipv4;

				switch(icmp->type()) {
				case ICMP_ECHO_REQUEST:
					cout << "EchoRequest(";
					break;
				case ICMP_ECHO_REPLY:
					cout << "EchoReply(";
					done = true;
					break;
				case ICMP_TIME_EXCEEDED:
					cout << "TimeExceeded(";
					break;
				default:
					cout << "Unknown(";
				}

				cout << "ttl=" << (ttl-1);
				cout << ",from=" << ipv4->source().hostname();
				cout << ",to=" << ipv4->destination().hostname();
				cout << ",ms=" << elapsed;
				cout << ")" << endl;
			}
			sleep(1);
		}
		return 0;
	}
};

int main(int argc, char *argv[]) {
	return TraceApplication::execute(argc, argv);
}
