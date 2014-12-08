
#ifndef BARRY_COMMON_H
#define BARRY_COMMON_H

#include <string.h>

/**
 * string equality check
 */

#define EQ(a, b) (0 == strcmp(a, b))

/**
 * barry program version
 */

#define BARRY_VERSION "0.0.1"

/**
 * output to `stdout'
 */

#define echo(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

/**
 * output to `stderr'
 */

#define error(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

/**
 * iterate over each item in `arr' with
 * `opt' calling `body' each time
 */

#define foreach(arr, value, body) ({             \
  int i = -1;                                    \
  while (arr[++i]) { value = arr[i]; body;  }    \
})

#endif
