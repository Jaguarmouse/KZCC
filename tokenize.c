#include "kzcc.h"

char *user_input;
Token *token;

void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

bool consume(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      strncmp(token->str, op, token->len)) 
    return false;
  token = token->next;
  return true;
}

Token *consume_ident() {
  if (token->kind != TK_IDENT)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
}

void expect(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      strncmp(token->str, op, token->len))
    error_at(token->str, "'%c'ではありません", *op);
  token = token->next;
}

int expect_number() {
  if (token->kind != TK_NUM)
    error_at(token->str, "数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

static bool is_alphabet(char c) {
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_';
}

static char *starts_with_reserved(char *p) {
  static char *kws[] = {"return", "if", "else", "while", "for"};

  for (int i = 0; i < sizeof(kws) / sizeof(*kws); i++) {
    int len = strlen(kws[i]);
    if (strncmp(kws[i], p, len) == 0 && !is_alphabet(p[len]))
      return kws[i];
  }

  static char *ops[] = {"<=", ">=", "==", "!="};

  for (int i = 0; i < sizeof(ops) / sizeof(*ops); i++)
    if (strncmp(ops[i], p, strlen(ops[i])) == 0)
        return ops[i];

  return NULL;
}

void tokenize() {
  char *p = user_input;

  Token head;
  head.next = NULL;
  Token *cur = &head;

  while(*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    char *op = starts_with_reserved(p);
    if (op) {
      int len = strlen(op);
      cur = new_token(TK_RESERVED, cur, p, len);
      p += len;
      continue;
    }

    if (is_alphabet(*p)) {
      char *q = p++;
      while (is_alphabet(*p))
        p++;
      cur = new_token(TK_IDENT, cur, q, p-q);
      continue;
    }

    if (ispunct(*p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      char *q = p;
      int val = strtol(p, &p, 10);
      cur = new_token(TK_NUM, cur, q, p-q);
      cur->val = val;
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p, 0);
  token = head.next;
}
