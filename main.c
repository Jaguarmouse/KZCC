#include "kzcc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    error("引数の個数が正しくありません。\n");
    return 1;
  }

  user_input = argv[1];
  tokenize();
  program();

  int stack_size = 0;
  for (LVar *lvar = locals; lvar; lvar = lvar->next)
    stack_size += 8;

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, %d\n", stack_size);

  for (int i = 0; code[i]; i++) {
    gen(code[i]);
    printf("  pop rax\n");
  }

  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}
