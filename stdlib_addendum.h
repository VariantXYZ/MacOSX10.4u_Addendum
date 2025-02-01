#ifndef __MACOSX_TIGER_STDLIB_ADDENDUM__
#define __MACOSX_TIGER_STDLIB_ADDENDUM__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// The Mac OS X 10.4u SDK has arc4random but not arc4random_buf
uint32_t arc4random(void);

static void arc4random_buf(void *buf, size_t nbytes)
{
	while (nbytes > 0)
	{
		uint32_t random = arc4random();
		size_t to_copy = nbytes > sizeof(random) ? sizeof(random) : nbytes;
		memcpy(buf, &random, to_copy);
		nbytes -= to_copy;
	}
}

#endif // __MACOSX_TIGER_STDLIB_ADDENDUM__