
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "global.h"
#include "common.h"

barry_global_t *BARRY_GLOBAL;

void *
barry_print (barry_function_arguments_t);

void *
barry_printf (barry_function_arguments_t);

void
barry_init_globals () {
  if (NULL == BARRY_GLOBAL) {
    BARRY_GLOBAL = (barry_global_t *) malloc(sizeof(barry_global_t));
    if (NULL == BARRY_GLOBAL) {
      error("Memory allocation error");
      exit(1);
    }
  }

  BARRY_GLOBAL->function_length = 0;
  BARRY_GLOBAL->decl_length = 0;

  BARRY_FUNCTION("print", barry_print);
}

void *
barry_print (barry_function_arguments_t arguments) {
  int i = 0;
  size_t size = arguments.length;
  for (; i < size; ++i) {
    printf("%s", arguments.values[i]);
  }
  printf("\n");
  return NULL;
}
