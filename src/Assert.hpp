#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <Exception.hpp>

class Assert {
public:
	static void notNull(const void *object, const char *file, const char *function, int line);
};

#endif /* __ASSERT_H__ */
