#include "block.h"
#include "sha256.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void calculate_hash(Block *block, char *output) {
    SHA256_CTX ctx;
    uint8_t hash[SHA256_BLOCK_SIZE];
    char data[1024];
    int pos = 0;
    
    pos += snprintf(data + pos, sizeof(data) - pos, "%d", block->index);
    pos += snprintf(data + pos, sizeof(data) - pos, "%ld", block->timestamp);
    pos += snprintf(data + pos, sizeof(data) - pos, "%s", block->previous_hash);
    pos += snprintf(data + pos, sizeof(data) - pos, "%d", block->nonce);
    
    for (int i = 0; i < block->transaction_count; i++) {
        pos += snprintf(data + pos, sizeof(data) - pos, "%s", block->transactions[i]);
    }
    
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)data, strlen(data));
    sha256_final(&ctx, hash);
    
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

void mine_block(Block *block, int difficulty) {
    char target[difficulty + 1];
    memset(target, '0', difficulty);
    target[difficulty] = '\0';
    
    while (1) {
        calculate_hash(block, block->hash);
        if (strncmp(block->hash, target, difficulty) == 0) {
            break;
        }
        block->nonce++;
    }
}

void print_block(const Block *block) {
    printf("Block #%d\n", block->index);
    printf("Timestamp: %ld\n", block->timestamp);
    printf("Previous Hash: %s\n", block->previous_hash);
    printf("Nonce: %d\n", block->nonce);
    printf("Hash: %s\n", block->hash);
    printf("Transactions:\n");
    for (int i = 0; i < block->transaction_count; i++) {
        printf("- %s\n", block->transactions[i]);
    }
    printf("\n");
}
