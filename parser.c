
#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "common.h"

int
barry_parse (char *file, char *src) {
  barry_lexer_t *lexer = (barry_lexer_t *) malloc(sizeof(barry_lexer_t));

  if (NULL == lexer) {
    error("error: lexer allocation error\n");
    return 1;
  }

  lexer->file = file;
  lexer->src = src;

  lexer->curr.id = TOK_NONE;
  lexer->curr.as.string = "";

  lexer->colno = 0;
  lexer->lineno = 1;
  lexer->offset = 0;

  while (0 == barry_lexer_scan(lexer)) {
    echo("token (%d, %d): %s\n",
        lexer->colno,
        lexer->lineno,
        lexer->curr.as.string);
  }

  return 0;
}
