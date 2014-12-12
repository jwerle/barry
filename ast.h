
#ifndef BARRY_AST_H
#define BARRY_AST_H

#define BARRAY_AST_NODE_MAX 1024
#define BARRY_SCOPE_MAX_DECL 1024

#include "token.h"

struct barry_scope_s;
struct barry_node_s;
struct barry_ast_s;

typedef struct {
  void *values[32];
  int length;
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
  const char *name;
  const char *body;
  struct barry_scope_s *locals;
  struct barry_scope_s *global;
} barry_def_t;

typedef struct barry_scope_s {
  barry_declaration_t decls[BARRY_SCOPE_MAX_DECL];
  barry_function_t functions[BARRY_SCOPE_MAX_DECL];
  barry_def_t definitions[BARRY_SCOPE_MAX_DECL];

  size_t decl_length;
  size_t function_length;
  size_t definition_length;
} barry_scope_t;

typedef struct barry_node_s {
  barry_token_t token;
  barry_scope_t *scope;
  struct barry_ast_s *ast;
  struct barry_node_s *prev;
  struct barry_node_s *next;
  void *value;
} barry_node_t;

typedef struct barry_ast_s {
  char *file;
  size_t length;
  int offset;
  barry_node_t *nodes[BARRAY_AST_NODE_MAX];
  barry_node_t *current;
} barry_ast_t;

#endif
