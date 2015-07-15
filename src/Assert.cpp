
#include <Assert.hpp>

void Assert::notNull(const void *object, const char *file, const char *function, int line) {
	if(object == 0) {
		throw Exception("Null pointer reference", file, function, line);
	}
}
void Assert::True(bool expression, const char *file, const char *function, int line) {
	if(!expression) {
		throw Exception("Null pointer reference", file, function, line);
	}
}
