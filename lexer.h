
#ifndef BARRY_LEXER_H
#define BARRY_LEXER_H

#include "token.h"

typedef struct barry_lexer {
  char *src;
  char *file;

  char pch;
  char ch;

  int colno;
  int lineno;
  int offset;

  barry_token_t last;
  barry_token_t curr;

} barry_lexer_t;

int
barry_lexer_scan (barry_lexer_t *);

#endif
