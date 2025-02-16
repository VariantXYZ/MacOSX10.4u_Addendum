#ifndef __MACOSX_TIGER_STDLIB_ADDENDUM__
#define __MACOSX_TIGER_STDLIB_ADDENDUM__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

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

// Provide a getline implementation
// I just pulled this from https://stackoverflow.com/a/4161605
static ssize_t getline(char **linep, size_t *np, FILE *stream)
{
  char *p = NULL;
  size_t i = 0;

  if (!linep || !np) {
    errno = EINVAL;
    return -1;
  }

  if (!(*linep) || !(*np)) {
    *np = 120;
    *linep = (char *)malloc(*np);
    if (!(*linep)) {
      return -1;
    }
  }

  flockfile(stream);

  p = *linep;
  for (int ch = 0; (ch = getc_unlocked(stream)) != EOF;) {
    if (i > *np) {
      /* Grow *linep. */
      size_t m = *np * 2;
      char *s = (char *)realloc(*linep, m);

      if (!s) {
        int error = errno;
        funlockfile(stream);
        errno = error;
        return -1;
      }

      *linep = s;
      *np = m;
    }

    p[i] = ch;
    if ('\n' == ch) break;
    i += 1;
  }
  funlockfile(stream);

  /* Null-terminate the string. */
  if (i > *np) {
    /* Grow *linep. */
      size_t m = *np * 2;
      char *s = (char *)realloc(*linep, m);

      if (!s) {
        return -1;
      }

      *linep = s;
      *np = m;
  }

  p[i + 1] = '\0';
  return ((i > 0)? i : -1);
}

#endif // __MACOSX_TIGER_STDLIB_ADDENDUM__