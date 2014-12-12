
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "global.h"
#include "common.h"

static int
parse_node (barry_node_t *);

static int
is_function (barry_node_t *node) {
  int i = 0;
  if (TOK_IDENTIFIER != node->token.type) {
    return 0;
  }

  if (TOK_LPAREN != node->next->token.type) {
    return 0;
  }

#define fn BARRY_GLOBAL->functions[i]
  for (;i < BARRY_GLOBAL->function_length; ++i) {
    if (EQ(node->token.as.string, fn.name)) {
      return 1;
    }
  }
#undef fn

  return 0;
}

static int
is_assignment (barry_node_t *node) {
  if (TOK_EQUAL == node->next->token.type) {
    return 1;
  }
  return 0;
}

static int
is_declaration (barry_node_t *node) {
  int i = 0;

  if (TOK_IDENTIFIER != node->token.type) {
    return 0;
  }

#define var BARRY_GLOBAL->decls[i]
  for (;i < BARRY_GLOBAL->decl_length; ++i) {
    if (EQ(node->token.as.string, var.key)) {
      return 1;
    }
  }
#undef var
  return 0;
}

static int
assign_declaration_node (barry_node_t *node) {
  char *key = node->token.as.string;
  void *value = node->next->next->token.as.string;

  if (NULL == value) {
    return 1;
  }

  BARRY_DECLARTION(key, value);
  return 0;
}

static barry_function_t *
get_function (const char *name) {
  int i = 0;
#define fn BARRY_GLOBAL->functions[i]
  for (;i < BARRY_GLOBAL->function_length; ++i) {
    if (EQ(name, fn.name)) {
      return &fn;
    }
  }
#undef fn
  return NULL;
}

static void *
get_declaration (const char *key) {
  int i = 0;
#define var BARRY_GLOBAL->decls[i]
  for (;i < BARRY_GLOBAL->decl_length; ++i) {
    if (EQ(key, var.key)) {
      return var.value;
    }
  }
#undef var
  return NULL;
}

static int
call_function_node (barry_node_t *node) {
  barry_function_arguments_t arguments;
  barry_node_t *next = node->next;

  if (TOK_LPAREN != next->token.type) {
    return 1;
  }

  next = node->next;

  if (NULL == next) {
    error("Unexpected end of input");
    return 1;
  }

#define push(v) arguments.values[arguments.length++] = v;

  while (TOK_RPAREN != next->token.type) {
    next = next->next;
    switch (next->token.type) {
      case TOK_STRING:
        push(next->token.as.string);
        break;

      case TOK_IDENTIFIER:
        parse_node(next);

        if (is_function(next)) {
          if (call_function_node(node) > 0) {
            return 1;
          }

          break;
        }

        if (is_declaration(next)) {
          push(get_declaration(next->token.as.string));
        }

        break;
    }
  }

#undef push

  barry_function_t *fn = get_function(node->token.as.string);

  if (NULL == fn) {
    return 1;
  }

  fn->function(arguments);

  return 0;
}

static int
parse_node (barry_node_t *node) {
  switch (node->token.type) {
    case TOK_NONE:
      break;

    case TOK_IDENTIFIER:
      if (is_function(node)) {
        if (call_function_node(node) > 0) {
          return 1;
        }

        break;
      }

      if (is_assignment(node)) {
        if (assign_declaration_node(node) > 0) {
          return 1;
        }

        break;
      }
      break;
  }

  return 0;
}

int
barry_eval (barry_ast_t *ast) {
  int i = 0;
  for (; i < ast->length; ++i) {
    barry_node_t *node = ast->nodes[i];
    if (parse_node(node) > 0) {
      return 1;
    }
  }
  return 0;
}

int
barry_parse (char *file, char *src) {
  barry_lexer_t *lexer = (barry_lexer_t *) malloc(sizeof(barry_lexer_t));
  barry_ast_t *ast = (barry_ast_t *) malloc(sizeof(barry_ast_t));

  if (NULL == lexer) {
    error("error: lexer allocation error\n");
    return 1;
  }

  if (NULL == ast) {
    error("error: ast allocation error\n");
    return 1;
  }

  lexer->src = src;
  lexer->file = file;
  lexer->colno = 0;
  lexer->lineno = 1;
  lexer->offset = 0;
  lexer->curr.type = TOK_NONE;
  lexer->curr.as.string = "";

  ast->file = file;

  // build ast
  while (0 == barry_lexer_scan(lexer)) {
    barry_node_t *node = (barry_node_t *) malloc(sizeof(barry_node_t));
    node->token = lexer->curr;

    if (ast->length > 0) {
      ast->nodes[ast->length - 1]->next = node;
    }

    ast->nodes[ast->length++] = node;
  }

  free(lexer);

  // parse ast
  return barry_eval(ast);
}
