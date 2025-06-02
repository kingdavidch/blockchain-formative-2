#ifndef BLOCK_H
#define BLOCK_H

#include <time.h>
#include <stdint.h>

#define MAX_TRANSACTIONS 10
#define MAX_DATA_LENGTH 256

typedef struct {
    int index;
    time_t timestamp;
    char transactions[MAX_TRANSACTIONS][MAX_DATA_LENGTH];
    int transaction_count;
    char previous_hash[65];
    int nonce;
    char hash[65];
} Block;

void calculate_hash(Block *block, char *output);
void mine_block(Block *block, int difficulty);
void print_block(const Block *block);

#endif
