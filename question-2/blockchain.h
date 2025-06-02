#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"

#define BLOCKCHAIN_SIZE 100

typedef struct {
    Block blocks[BLOCKCHAIN_SIZE];
    int length;
    int difficulty;
} Blockchain;

void init_blockchain(Blockchain *bc, int difficulty);
int add_block(Blockchain *bc, Block *new_block);
int is_chain_valid(const Blockchain *bc);
void print_blockchain(const Blockchain *bc);

#endif
