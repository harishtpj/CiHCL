#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

HVM vm;

static void resetStack() {
  vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.pc++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

  while (true) {
    #ifdef DEBUG_TRACE_EXECUTION
      printf("          ");
      for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
      }
      printf("\n");
      disassembleInstruction(vm.chunk, (int)(vm.pc - vm.chunk->code));
    #endif

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
        case OP_CONST: {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }

        case OP_RET: {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        
        case OP_NEGATE:   push(-pop()); break;
        case OP_ADD:      BINARY_OP(+); break;
        case OP_SUB: BINARY_OP(-); break;
        case OP_MUL: BINARY_OP(*); break;
        case OP_DIV:   BINARY_OP(/); break;  
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.pc = vm.chunk->code;
  return run();
}