#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

/* C++ headers */
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

/* C headers */
#include <string.h>

class Exception : public std::exception {
public:
	Exception() noexcept;
	Exception(const char *message) noexcept;
	Exception(const char *message, const char *function, const char *file, int line) noexcept;
	Exception(const std::string &message) noexcept;
	Exception(const std::string &message, const char *function, const char *file, int line) noexcept;
	void initializeExceptionMessageWith(const char *text, const char *function, const char *file, int line);
	virtual const char* what() const noexcept;
	friend std::ostream& operator<<(std::ostream &ostream, const Exception &exception);
protected:
	std::string _exceptionMessage;
};

std::ostream& operator<<(std::ostream &ostream, const Exception &exception);

#define FullException(message) Exception(message, __FUNCTION__, __FILE__, __LINE__)

#endif /* __EXCEPTION_H__ */
