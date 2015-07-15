#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

/* C++ headers */
#include <exception>
#include <iostream>
#include <string>

class Exception : public std::exception {
public:
	Exception() noexcept;
	Exception(const char *exceptionMessage) noexcept;
	Exception(const std::string &exceptionMessage) noexcept;
	virtual const char* what() const noexcept;
	friend std::ostream& operator<<(std::ostream &ostream, const Exception &exception);
protected:
	std::string _exceptionMessage;
};

std::ostream& operator<<(std::ostream &ostream, const Exception &exception);

#endif /* __EXCEPTION_H__ */
