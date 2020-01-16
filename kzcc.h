#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 
// tokenize.c
// 

typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident(void);
void expect(char *op);
int expect_number(void);
bool at_eof(void);
void tokenize(void);

extern char *user_input;
extern Token *token;

// 
// parse.c
//

typedef struct LVar LVar;

struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_LT,
  ND_LE,
  ND_EQ,
  ND_NE,
  ND_ASSIGN,
  ND_RETURN,
  ND_IF,
  ND_WHILE,
  ND_FOR,
  ND_BLOCK,
  ND_LVAR,
  ND_NUM
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;

  //ND_NUM
  int val;

  //ND_LVAR
  int offset;

  //ND_IF, ND_WHILE, ND_FOR
  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  //ND_BLOCK
  Node *next;
  Node *body;
};

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

extern Node *code[];
extern LVar *locals;

// 
// codegen.c
//

void gen_lval(Node *node);
void gen(Node *node);
