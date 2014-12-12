
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "global.h"
#include "common.h"

barry_scope_t *BARRY_GLOBAL;

static void *
barry_print (barry_function_arguments_t);

static void *
barry_printf (barry_function_arguments_t);

void
BARRY_DECLARTION (const char *key, void *value) {
  int i = 0;
  size_t len = BARRY_GLOBAL->decl_length;
  for (; i < len; ++i) {
    if (EQ(key, BARRY_GLOBAL->decls[i].key)) {
      BARRY_GLOBAL->decls[i].value = value;
      break;
    }
  }

  BARRY_GLOBAL->decls[BARRY_GLOBAL->decl_length++] =
    (barry_declaration_t) { .key = key, .value = value };
}

void
BARRY_DEFINITION (const char *name, const char *body) {
  int i = 0;
  size_t len = BARRY_GLOBAL->definition_length;
  for (; i < len; ++i) {
    if (EQ(name, BARRY_GLOBAL->definitions[i].name)) {
      BARRY_GLOBAL->definitions[i].body = body;
      break;
    }
  }

  BARRY_GLOBAL->definitions[BARRY_GLOBAL->definition_length++] =
    (barry_def_t) {
      .name = name,
      .body = body,
      .locals = (barry_scope_t *) malloc(sizeof(barry_scope_t)),
      .global = BARRY_GLOBAL,
    };
}

void
barry_init_globals () {
  if (NULL == BARRY_GLOBAL) {
    BARRY_GLOBAL = (barry_scope_t *) malloc(sizeof(barry_scope_t));
    if (NULL == BARRY_GLOBAL) {
      error("Memory allocation error");
      exit(1);
    }
  }

  BARRY_GLOBAL->function_length = 0;
  BARRY_GLOBAL->decl_length = 0;

  BARRY_BUILTIN_FUNCTION("print", barry_print);
}

static void *
barry_print (barry_function_arguments_t arguments) {
  int i = 0;
  size_t size = arguments.length;
  for (; i < size; ++i) {
    printf("%s", arguments.values[i]);
    if (i < size - 1) {
      printf(" ");
    }
  }
  printf("\n");
  return NULL;
}
