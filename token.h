
#ifndef BARRY_TOKEN_H
#define BARRY_TOKEN_H

#define TOK_NONE 0x0000
#define TOK_LPAREN 0x0001
#define TOK_RPAREN 0x0002
#define TOK_SEMICOLON 0x0003
#define TOK_IDENTIFIER 0x0004
#define TOK_STRING 0x0005
#define TOK_PLUS 0x0006
#define TOK_MINUS 0x0007
#define TOK_MULTIPLY 0x0008
#define TOK_DIVIDE 0x0009
#define TOK_EQUAL 0x0010
#define TOK_NEGATE 0x0011
#define TOK_PIPE 0x0012

typedef struct barry_token {
  int type;
  int colno;
  int lineno;

  char *name;

  union {
    char *string;
    unsigned int integer;
  } as;

} barry_token_t;

#endif
