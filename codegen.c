#include "kzcc.h"

int beginseq = 0;
int endseq = 0;
int elseseq = 0;
int callseq = 0;

static char *argreg8[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("left value not variable!");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node) {
  // Label ids
  int begin,end,els, call;

  // Argument count
  int nargs;

  // Iterators
  int i;

  switch (node->kind) {
  case ND_NUM:
    printf("  push %d\n", node->val);
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
 case ND_RETURN:
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  case ND_IF:
    els = elseseq++;
    end = endseq++;
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    if (node->els) {
      printf("  je .L.else.%d\n", els);
      gen(node->then);
      printf("  jmp .L.end.%d\n", end);
      printf(".L.else.%d:\n", els);
      gen(node->els);
    } else {
      printf("  je .L.end.%d\n", end);
      gen(node->then);
    }
    printf(".L.end.%d:\n", end);
    return;
  case ND_WHILE:
    begin = beginseq++;
    end = endseq++;
    printf(".L.begin.%d:\n", begin);
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L.end.%d\n", end);
    gen(node->then);
    printf("  jmp .L.begin.%d\n", begin);
    printf(".L.end.%d:\n", end);
    return;
  case ND_FOR:
    begin = beginseq++;
    end = endseq++;
    gen(node->init);
    printf(".L.begin.%d:\n", begin);
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L.end.%d\n", end);
    gen(node->then);
    gen(node->inc);
    printf("  jmp .L.begin.%d\n", begin);
    printf(".L.end.%d:\n", end);
    return;
  case ND_BLOCK:
    for (Node *n = node->body; n; n = n->next)
      gen(n);
    return;
  case ND_FUNCALL:
    nargs = 0;
    for (Node *arg = node->args; arg; arg = arg->next) {
      gen(arg);
      nargs++;
    }

    for (i = nargs - 1; i >= 0; i--)
      printf("  pop %s\n", argreg8[i]);

    call = callseq++;
    end = endseq++;
    
    // check if rsp aligns to 16 (ABI requirement)
    printf("  mov rax, rsp\n");
    printf("  and rax, 15\n");

    // if not, go do that
    printf("  jnz .L.call.%d\n", call);

    // else, reset rax and call
    printf("  mov rax, 0\n");
    printf("  call %s\n", node->funcname);

    // jump to the end
    printf("  jmp .L.end.%d\n", end);

    // make sure rsp aligns to 16
    printf(".L.call.%d:\n", call);
    printf("  sub rsp, 8\n");
    
    // reset rax and call
    printf("  mov rax, 0\n");
    printf("  call %s\n", node->funcname);

    // set rsp back to its original address
    printf("  add rsp, 8\n");
    printf(".L.end.%d:\n", end);

    // push the return value to the stack top
    printf("  push rax\n");

    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}
