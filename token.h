
#ifndef BARRY_TOKEN_H
#define BARRY_TOKEN_H

#define TOK_NONE 0x0000
#define TOK_LPAREN 0x0001
#define TOK_RPAREN 0x0002
#define TOK_SEMICOLON 0x0003
#define TOK_IDENTIFIER 0x0004
#define TOK_STRING 0x0005

typedef struct barry_token {
  int id;
  char *name;

  union {
    char *string;
    unsigned int integer;
  } as;

} barry_token_t;

#endif
