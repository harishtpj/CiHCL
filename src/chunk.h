#ifndef HCLANG_CHUNK_H
#define HCLANG_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONST,
    OP_RET,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);

#endif