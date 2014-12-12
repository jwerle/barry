
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
#define TOK_NUMBER 0x0013
#define TOK_DEF 0x0014
#define TOK_LBRACE 0x0015
#define TOK_RBRACE 0x0016
#define TOK_LBRACKET 0x0017
#define TOK_RBRACKET 0x0018
#define TOK_DO 0x0019
#define TOK_END 0x0020
#define TOK_COMMA 0x0021

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
