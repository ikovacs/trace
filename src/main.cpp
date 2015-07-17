/* C++ */
#include <iostream>
#include <NetworkInterface.hpp>
#include <InternetAddress.hpp>
#include <Socket.hpp>
#include <Packet.hpp>
#include <Clock.hpp>

using namespace std;

/*
	TODO:
	· Solucionar problema con la interfaz de loop.
 		"Exception: Address family not supported by protocol at: send (src/Socket.cpp:26)"
	· Solucionar tema de diseño Paquetes.
		"Qué pasa cuando recibimos un IPv4 con tamaño != 20 bytes. Crear un parser de paquetes."
*/

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
		EchoRequest echoRequest;
		echoRequest.source(ifaces[1].address());
		echoRequest.destination(addresses[0].address());
		echoRequest.timeToLive(64);

		Packet answer;
		Clock clock;

		bool done = false;
		unsigned char ttl = 1;
		unsigned short sequenceNumber = 0;
		uint64 elapsed;

		while((!done) and (ttl < 256)) {

			echoRequest.timeToLive(ttl++);
			echoRequest.sequenceNumber(sequenceNumber++);

			try {
				clock.start();
				socket.send(addresses[0].address(), echoRequest);
				socket.receive(answer);
				elapsed = clock.elapsed();
			}
			catch(Exception &exception) {
				cout << "***" << endl;
				continue;
			}

			IPv4 *ipv4 = (IPv4 *) &answer;

			if(ipv4->protocol() == IPV4_PROTO_ICMP) {

				ICMPv4 *icmp = (ICMPv4 *) ipv4;
				Echo *echo = (Echo *) icmp;

				switch(icmp->type()) {
				case ICMP_ECHO_REQUEST:
					cout << "EchoRequest(";
					break;
				case ICMP_ECHO_REPLY:
					cout << "EchoReply(";
					done = (echo->sequenceNumber() == echoRequest.sequenceNumber()) and (echo->identifier() == echoRequest.identifier());
					break;
				case ICMP_TIME_EXCEEDED:
					cout << "TimeExceeded(";
					break;
				default:
					cout << "Unknown(";
				}
				cout << "id=" << echo->identifier();
				cout << ",seq=" << echo->sequenceNumber();
				cout << ",ttl=" << (ttl-1);
				cout << ",from=" << ipv4->source().hostname();
				cout << ",to=" << ipv4->destination().hostname();
				cout << ",ms=" << elapsed;
				cout << ")" << endl;
			}
		}
		return 0;
	}
};

int main(int argc, char *argv[]) {
	return TraceApplication::execute(argc, argv);
}
