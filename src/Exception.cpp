
#include <Exception.hpp>

Exception::Exception() noexcept {}
Exception::Exception(const char *message) noexcept : std::exception(), _exceptionMessage(message) {}
Exception::Exception(const char *message, const char *function, const char *file, int line) noexcept : std::exception() {
	this->initializeExceptionMessageWith(message, function, file, line);
}
Exception::Exception(const std::string &message) noexcept : std::exception(), _exceptionMessage(message) {}
Exception::Exception(const std::string &message, const char *function, const char *file, int line) noexcept : std::exception() {
	this->initializeExceptionMessageWith(message.c_str(), function, file, line);
}
void Exception::initializeExceptionMessageWith(const char *text, const char *function, const char *file, int line) {
	std::ostringstream message;
	message << text << " at: " <<  function << " (" << file << ":" << line << ")";
	_exceptionMessage = message.str();
}
const char* Exception::what() const noexcept {
	return _exceptionMessage.c_str();
}
std::ostream& operator<<(std::ostream &ostream, const Exception &exception) {
	ostream << "Exception: " << exception._exceptionMessage;
	return ostream;
}
