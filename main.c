
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fs/fs.h>

#include "global.h"
#include "parser.h"
#include "common.h"

/**
 * output usage
 */

static void
usage (int err) {
  char *str = "usage: barry [-hV] <file>";
  if (err) {
    error("%s\n", str);
  } else {
    echo("%s\n", str);
  }
}

/**
 * `main()'
 */

int
main (int argc, char **argv) {
  char *file = NULL;
  char *src = NULL;

  // no arguments
  if (argc < 2) {
    usage(1);
    return 1;
  }

  // parse arguments
  (void) *argv++;
  foreach(argv, char *opt, {
    // parse opts
    if ('-' == opt[0]) {
      if (EQ(opt, "-h") || EQ(opt, "--help")) {
        usage(0);
        exit(0);
      } else if (EQ(opt, "-V") || EQ(opt, "--version")) {
        echo("barry v%s\n", BARRY_VERSION);
        exit(0);
      } else {
        error("error: Unknown option `%s'\n", opt);
        usage(1);
        exit(1);
      }

      continue;
    }

    file = opt;
    break;
  });

  if (0 != fs_exists(file)) {
    error("error: Cannot find file `%s'\n", file);
    return 1;
  }

  src = fs_read(file);

  if (NULL == src) {
    error("error: Error reading `%s'\n", file);
    return 1;
  }

  barry_init_globals();
  return barry_parse(file, src);
}
