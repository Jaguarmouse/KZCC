#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//token値型
enum {
  TK_NUM = 256,
  TK_EOF,
};

//token
typedef struct Token {
  int ty; //type
  int val; //数値
  char *input; //エラー用文字列
} Token;

//node値型
enum {
  ND_NUM = 256,
};

//node
typedef struct Node {
  int ty;
  int val;
  struct Node *lhs;
  struct Node *rhs;
} Node;

Node *add();
Node *mul();
Node *term();
void error(char*, char*);

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

Token tokens[100];
int pos = 0;

void tokenize(char *p) {
  int i = 0;
  while (*p) {
    //空白
    if (isspace(*p)) {
      p++;
      continue;
    }


    //数値(int)
    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    } //演算子(+ or -)
    else {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    error("トークナイズできません: %s\n", p);
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

int consume(int ty) {
  if (tokens[pos].ty != ty)
    return 0;
  pos++;
  return 1;
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node('+', node, mul());
    else if (consume('-'))
      node = new_node('-', node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = term();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, term());
    else if (consume('/'))
      node = new_node('/', node, term());
    else
      return node;
  }
}

Node *term() {
  if (consume('(')) {
    Node *node = add();
    if (!consume(')'))
      error("開きカッコに対応する閉じカッコがありません: %s", tokens[pos].input);
    return node;
  }

  if (tokens[pos].ty == TK_NUM)
    return new_node_num(tokens[pos++].val);

  error("数値でも開きカッコでもないトークンです: %s", tokens[pos].input);
}

void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->ty) {
  case '+':
    printf("  add rax, rdi\n");
    break;
  case '-':
    printf("  sub rax, rdi\n");
    break;
  case '*':
    printf("  mul rdi\n");
    break;
  case '/':
    printf("  mov rdx, 0\n");
    printf("  div rdi\n");
  }

  printf("  push rax\n");
}

void error(char *s, char *e) {
  fprintf(stderr, s, e);
  exit(1);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません。\n");
    return 1;
  }

  tokenize(argv[1]);
  Node *node = add();

  printf(".intel_syntax noprefix\n");
  printf(".global _main\n");
  printf("_main:\n");

  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
