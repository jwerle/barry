
#include <math.h>
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

// see: http://stackoverflow.com/a/2303798
static char *
ftoa (float num) {
  int i = 0;
  char fstr[80];
  int m = log10(num);
  int digit;
  float tolerance = .0001f;
  float precision = 0.00000000000001;

  while (num > 0 + precision) {
    float weight = pow(10.0f, m);
    digit = floor(num / weight);
    num -= (digit*weight);
    fstr[i++] = '0' + digit;
    if (m == 0)
      fstr[i++] = '.';
    m--;
  }

  fstr[i] = '\0';
  return strdup(fstr);
}

static int
is_function (barry_node_t *node) {
  if (TOK_IDENTIFIER != node->token.type) {
    return 0;
  }

  if (TOK_LPAREN != node->next->token.type) {
    return 0;
  }

  return 1;
}

static int
is_function_declaration (barry_node_t *node) {
  barry_node_t *next = node->next;
  char *name = NULL;

  if (TOK_IDENTIFIER != node->token.type) {
    return 0;
  }

  if (!EQ("def", node->token.as.string)) {
    return 0;
  }

  if (NULL == node->next) {
    error("Unexpected end of input");
    return 0;
  }

  name = next->token.as.string;
  next = next->next;

  if (NULL == name) {
    error("Unexpected token. Expected name.");
    return 0;
  }

  if (NULL == next) {
    error("Unexpected end of input");
    return 0;
  }

  if (TOK_LBRACE != next->token.type) {
    error("Unexpected token. Expected `('.");
    return 0;
  }

  next = next->next;

  if (NULL == next) {
    error("Unexpected end of input");
    return 0;
  }

  return 1;
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

  error("%s is not defined", key);
  return NULL;
}

static barry_def_t *
get_definition (const char *name) {
  int i = 0;
#define def BARRY_GLOBAL->definitions[i]
  for (;i < BARRY_GLOBAL->definition_length; ++i) {
    if (EQ(name, def.name)) {
      return &def;
    }
  }
#undef def
  return NULL;
}

static int
assign_declaration_node (barry_node_t *node) {
  barry_node_t *value = node->next->next;
  char *key = node->token.as.string;
  uint64_t *nv = NULL;
  void *v = NULL;

  if (NULL == value) {
    return 1;
  }

  switch (value->token.type) {
    case TOK_IDENTIFIER:
      v = get_declaration(value->token.as.string);
      BARRY_DECLARTION(key, v);
      break;

    case TOK_NUMBER:
    case TOK_STRING:
      v = value->token.as.string;
      BARRY_DECLARTION(key, v);
      break;
  }

  node->ast->current = value->next;

  return 0;
}

static int
assign_function_declaration_node (barry_node_t *node) {
  barry_node_t *next = node->next;
  const char *name = NULL;
  char body[BUFSIZ];
  int rbrace = 0;

  name = next->token.as.string;
  next = next->next;

  if (TOK_LBRACE != next->token.type) {
    error("Unexpected token. Expected `{'.");
    return 1;
  }

  next = next->next;
  memset(body, 0, BUFSIZ);
  while (TOK_RBRACE != next->token.type || rbrace > 0) {
    if (TOK_STRING == next->token.type) {
      strcat(body, "\"");
      strcat(body, next->token.as.string);
      strcat(body, "\"");
    } else {
      strcat(body, next->token.as.string);
    }

    next = next->next;
    if (TOK_LBRACE == next->token.type) {
      rbrace++;
      strcat(body, next->token.as.string);
      next = next->next;
    }

    if (TOK_RBRACE == next->token.type && rbrace > 0) {
      rbrace--;
      strcat(body, next->token.as.string);
      next = next->next;
    }
  }

  BARRY_DEFINITION(name, strdup(body));

  node->ast->current = next->next;
  return 0;
}

static int
call_function_node (barry_node_t *node) {
  barry_function_arguments_t arguments;
  barry_node_t *next = node->next;

  arguments.length = 0;

  if (TOK_LPAREN != next->token.type) {
    error("Unexpected token. Expected `('");
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
    node->ast->current = next;
    switch (next->token.type) {
      case TOK_NUMBER:
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

  node->ast->current = next->next;
  barry_function_t *fn = get_function(node->token.as.string);
  barry_def_t *def = NULL;

  if (NULL == fn) {
    def = get_definition(node->token.as.string);
    if (NULL == def) {
      error("%s is not a function", node->token.as.string);
    } else {
      return barry_parse((char *) def->name, (char *) def->body);
    }
    return 1;
  } else {
    fn->function(arguments);
  }

  return 0;
}

static int
parse_node (barry_node_t *node) {
  switch (node->token.type) {
    case TOK_NONE:
      break;

    case TOK_IDENTIFIER:
      // function call
      if (is_function(node)) {
        if (call_function_node(node) > 0) {
          return 1;
        }

        break;
      }

      // assignment `var = value'
      if (is_assignment(node)) {
        if (assign_declaration_node(node) > 0) {
          return 1;
        }

        break;
      }

      // function declaration
      if (is_function_declaration(node)) {
        if (assign_function_declaration_node(node)) {

        }
      }
      break;
  }

  return 0;
}

int
barry_eval (barry_ast_t *ast) {
  int i = 0;
  ast->current = ast->nodes[0];
  while (ast->current) {
    if (parse_node(ast->current) > 0) {
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
    node->ast = ast;

    if (ast->length > 0) {
      ast->nodes[ast->length - 1]->next = node;
    }

    ast->nodes[ast->length++] = node;
  }

  free(lexer);

  // parse ast
  return barry_eval(ast);
}
