
#include <InternetAddress.hpp>

InternetAddress::InternetAddress(const char *cannonicalName, const struct addrinfo *ai) {
	AssertNotNull(ai);
	_flags = ai->ai_flags;
	_family = (AddressFamily) ai->ai_family;
	_socketType = (SocketType) ai->ai_socktype;
	_protocol = ai->ai_protocol;
	AssertNotNull(ai->ai_addr);
	_address = new SocketAddress(ai->ai_addr);
	if(cannonicalName != 0)
		_canonicalName = cannonicalName;
}

InternetAddress::InternetAddress(const InternetAddress &internetAddress) {
	_flags = internetAddress._flags;
	_family = internetAddress._family;
	_socketType = internetAddress._socketType;
	_protocol = internetAddress._protocol;
	AssertNotNull(internetAddress._address);
	_address = new SocketAddress(*internetAddress._address);
	_canonicalName = internetAddress._canonicalName;
}

InternetAddress::~InternetAddress() {
	AssertNotNull(_address);
	delete _address;
}

SocketType InternetAddress::socketType() const {
	return _socketType;
}

const SocketAddress& InternetAddress::address() const {
	AssertNotNull(_address);
	return *_address;
}

std::string InternetAddress::cannonicalName() const {
	return _canonicalName;
}

std::vector<InternetAddress> InternetAddress::resolve(const char *node) {

	AssertNotNull(node);

	int answer;

	std::vector<InternetAddress> addresses;

	struct addrinfo hints, *aip, *ai;

	::memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;

	if((answer = ::getaddrinfo(node, ServiceNone, &hints, &aip)) != GETADDRINFO_SUCCESS)
		throw Exception(gai_strerror(answer));

	char *canonname = 0;

	if((aip != 0) and (aip->ai_canonname != 0))
		canonname = aip->ai_canonname;

	for(ai = aip; ai != 0; ai = ai->ai_next) {
		InternetAddress internetAddress(canonname, ai);
		addresses.push_back(internetAddress);
	}

	::freeaddrinfo(aip);
	return addresses;
}

std::ostream& operator<<(std::ostream &ostream, const InternetAddress &internetAddress) {
	ostream << "InternetAddress(";
	ostream << "name=" << internetAddress.cannonicalName();
	ostream << ",address=" << internetAddress.address().toString();
	switch(internetAddress.socketType()) {
	case Raw:
		ostream << ",socketType=Raw";
		break;
	case Datagram:
		ostream << ",socketType=Datagram";
		break;
	case Stream:
		ostream << ",socketType=Stream";
		break;
	default:
		ostream << ",socketType=Unknown";
		break;
	}
	ostream << ")";
	return ostream;
}
