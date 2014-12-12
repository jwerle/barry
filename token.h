
#ifndef BARRY_TOKEN_H
#define BARRY_TOKEN_H

enum {
#define X(tok) TOK_## tok
  X(NONE),
  // (
  X(LPAREN),
  // )
  X(RPAREN),
  // {
  X(LBRACE),
  // }
  X(RBRACE),
  // [
  X(LBRACKET),
  // ]
  X(RBRACKET),
  // ;
  X(SEMICOLON),
  // +
  X(PLUS),
  // -
  X(MINUS),
  // *
  X(MULTIPLY),
  // /
  X(DIVIDE),
  // =
  X(EQUAL),
  // ,
  X(COMMA),
  // ~
  X(NEGATE),
  // |
  X(PIPE),
  // |
  X(BITWISE_OR),
  // ~
  X(TILDE),
  // >>
  X(BITWISE_RSHIFT),
  // <<
  X(BITWISE_LSHIFT),
  // [a-zA-Z0-9]
  X(IDENTIFIER),
  // "string"
  X(STRING),
  // 0-9.0-9*
  X(NUMBER),
  // def <name>
  X(DEF),
  // do
  X(DO),
  // end
  X(END),
  // if
  X(IF),
  // else
  X(ELSE),
  // for
  X(FOR),
  // while
  X(WHILE),
  // break
  X(BREAK),
  // continue
  X(CONTINUE),
#undef X
};

typedef struct barry_token {
  int type;
  int colno;
  int lineno;

  char *name;

  struct {
    char *string;
    float number;
  } as;

} barry_token_t;

#endif
