
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "lexer.h"
#include "common.h"

#define peek(self) self->src[self->offset]

static unsigned char
next (barry_lexer_t *self) {
  unsigned char ch = 0;
  int idx = self->offset;

  ch = self->src[idx];

  if ('\0' != ch) {
    self->offset = idx + 1;
    self->pch = self->ch;
    self->ch = ch;

    if ('\n' == ch || '\r' == ch) {
      self->colno = 1;
    } else {
      self->colno++;
    }

    return ch;
  }

  return '\0';
}

static unsigned char
prev (barry_lexer_t *self) {
  unsigned char ch = 0;
  int idx = self->offset - 1;
  if (idx < 0) {
    self->offset = 0;
    self->colno = 1;
    self->ch = self->src[0];
    self->pch = 0;
    return self->src[0];
  } else {
    self->offset--;
    if (self->colno < 3) {
      self->colno = 1;
    } else {
      self->colno--;
    }

    self->ch = self->src[idx];
    if (0 == idx) {
      self->pch = 0;
    } else {
      self->pch = self->src[idx - 1];
    }
    return self->src[idx];
  }
}

static void
token (barry_lexer_t *self, int type, char *buf) {
  self->last = self->curr;
  self->curr.type = type;
  self->curr.as.string = strdup(buf);
  self->curr.lineno = self->lineno;
  self->curr.colno = self->colno;

  switch (type) {
    case TOK_NUMBER:
      self->curr.as.number = atof(buf);
      break;
  }
}

static int
scan_identifier (barry_lexer_t *self, unsigned char ch) {
  unsigned char buf[BUFSIZ];
  size_t size = 0;
  int num = 0;
  int set = 0;
  int i = 0;

  do {
    buf[size++] = ch;
    ch = next(self);
  } while (isalpha(ch) || isdigit(ch) || '_' == ch || '.' == ch);

  prev(self);

  if (0 == size) {
    // @TODO - handle with error
    return -1;
  }

  buf[size] = '\0';

  num = 1;
  for (; i < size; ++i) {
    if (!isdigit(buf[i]) && '.' != buf[i]) {
      num = 0;
    }
  }

#define SET_TOKEN_IF(str, tok)                             \
  if (0 == set && EQ((char * ) str, (char *) buf)) {       \
    set = 1; token(self, tok, (char *) buf);               \
  }

  if (num) {
    token(self, TOK_NUMBER, (char *) buf);
  } else {
    SET_TOKEN_IF("if", TOK_IF);
    SET_TOKEN_IF("do", TOK_DO);
    SET_TOKEN_IF("for", TOK_FOR);
    SET_TOKEN_IF("end", TOK_END);
    SET_TOKEN_IF("else", TOK_ELSE);
    SET_TOKEN_IF("while", TOK_WHILE);
    SET_TOKEN_IF("break", TOK_WHILE);
    SET_TOKEN_IF("continue", TOK_WHILE);
    SET_TOKEN_IF(buf, TOK_IDENTIFIER);
  }

#undef TOKEN

  return 0;
}

static int
scan_string (barry_lexer_t *self, unsigned char ch) {
  unsigned char quote = ch;
  unsigned char buf[BUFSIZ];
  size_t size = 0;
  int ignore = 0;

  // prevent accidental non-strings
  if ('"' != ch && '\'' != ch) {
    // @TODO - handle with syntax error
    return -1;
  }

  while (1) {
    ch = next(self);
    ignore = 0;

    if ('\\' == ch && ('"' == peek(self) || '\'' == peek(self))) {
      buf[size++] = ch;
      ch = next(self);
      ignore = 1;
    }

    if (quote == ch && 0 == ignore) {
      break;
    }

    if ('\n' == ch || '\r' == ch) {
      // @TODO - handle with syntax error
      return -1;
    }

    buf[size++] = ch;
  }

  buf[size] = '\0';
  self->colno -= size;
  token(self, TOK_STRING, (char *) buf);

  return 0;
}

int
barry_lexer_scan (barry_lexer_t *self) {
  unsigned char ch = 0;

scan:
  ch = next(self);
  switch (ch) {
    case '\0': return 1;
    case ' ': case '\t':
      goto scan;

    case '#':
      while (ch != '\n' && ch != '\r') { ch = next(self); }
      goto scan;

    case '\r': case '\n':
      self->lineno++;
      self->colno = 1;
      goto scan;

    case '"': case '\'':
      return scan_string(self, ch);

    case ',':
      return token(self, TOK_COMMA, ","), 0;

    case '(':
      return token(self, TOK_LPAREN, "("), 0;

    case ')':
      return token(self, TOK_RPAREN, ")"), 0;

    case ';':
      return token(self, TOK_SEMICOLON, ";"), 0;

    case '+':
      return token(self, TOK_PLUS, "+"), 0;

    case '-':
      return token(self, TOK_MINUS, "-"), 0;

    case '*':
      return token(self, TOK_MULTIPLY, "*"), 0;

    case '/':
      return token(self, TOK_DIVIDE, "/"), 0;

    case '=':
      return token(self, TOK_EQUAL, "="), 0;

    case '!':
      return token(self, TOK_NEGATE, "!"), 0;

    case '|':
      return token(self, TOK_PIPE, "|"), 0;

    case '{':
      return token(self, TOK_LBRACE, "{"), 0;

    case '}':
      return token(self, TOK_RBRACE, "}"), 0;

    case '[':
      return token(self, TOK_LBRACKET, "["), 0;

    case ']':
      return token(self, TOK_RBRACKET, "]"), 0;

    default: return scan_identifier(self, ch);
  }

  return 0;
}
