
#ifndef BARRY_GLOBAL_H
#define BARRY_GLOBAL_H

#include "ast.h"
#include "common.h"

#define BARRY_BUILTIN_FUNCTION(n, fn)                                \
  BARRY_GLOBAL->functions[BARRY_GLOBAL->function_length++] =         \
    (barry_function_t) {  .name = n, .function = fn, };

extern barry_scope_t *BARRY_GLOBAL;

void
barry_init_globals ();

void
BARRY_DECLARTION (const char *, void *);

void
BARRY_DEFINITION (const char *, const char *);

#endif
