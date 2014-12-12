
#ifndef BARRY_GLOBAL_H
#define BARRY_GLOBAL_H

#include "ast.h"

#define BARRY_FUNCTION(n, fn)                                        \
  BARRY_GLOBAL->functions[BARRY_GLOBAL->function_length++] =         \
    (barry_function_t) {  .name = n, .function = fn, };

#define BARRY_DECLARTION(k, v)                                       \
  BARRY_GLOBAL->decls[BARRY_GLOBAL->decl_length++] =                 \
    (barry_declaration_t) { .key = k, .value = (void *) v };

extern barry_global_t *BARRY_GLOBAL;

void
barry_init_globals ();

#endif
