
#ifndef BARRY_AST_H
#define BARRY_AST_H

#define BARRAY_AST_NODE_MAX 1024
#define BARRY_GLOBAL_MAX_DECL 1024

#include "token.h"

struct barry_node_s;
typedef struct {
  void *values[32];
  size_t length;
} barry_function_arguments_t;

typedef void * (* barry_function) (barry_function_arguments_t);

typedef struct {
  const char *name;
  barry_function function;
} barry_function_t;

typedef struct {
  const char *key;
  void *value;
} barry_declaration_t;

typedef struct {
  barry_declaration_t decls[BARRY_GLOBAL_MAX_DECL];
  barry_function_t functions[BARRY_GLOBAL_MAX_DECL];

  size_t decl_length;
  size_t function_length;
} barry_global_t;

typedef struct barry_node_s {
  barry_token_t token;
  struct barry_node_s *next;
  void *value;
} barry_node_t;

typedef struct {
  char *file;
  size_t length;
  barry_node_t *nodes[BARRAY_AST_NODE_MAX];
} barry_ast_t;

#endif
