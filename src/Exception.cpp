
#include <Exception.hpp>

Exception::Exception() noexcept {}
Exception::Exception(const char *exceptionMessage) noexcept : std::exception(), _exceptionMessage(exceptionMessage) {}
Exception::Exception(const std::string &exceptionMessage) noexcept : std::exception(), _exceptionMessage(exceptionMessage) {}
const char* Exception::what() const noexcept {
	return _exceptionMessage.c_str();
}
std::ostream& operator<<(std::ostream &ostream, const Exception &exception) {
	ostream << "Exception: " << exception._exceptionMessage;
	return ostream;
}
