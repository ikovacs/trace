#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <Exception.hpp>

class Assert {
public:
	static void True(bool expression, const char *file, const char *function, int line);
	static void notNull(const void *object, const char *file, const char *function, int line);
};

#define AssertTrue(expression) Assert::True(expression, __FILE__, __FUNCTION__, __LINE__)
#define AssertNotNull(object) Assert::notNull(object, __FILE__, __FUNCTION__, __LINE__)

#endif /* __ASSERT_H__ */
