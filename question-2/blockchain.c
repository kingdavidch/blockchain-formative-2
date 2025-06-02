#include "blockchain.h"
#include <string.h>
#include <time.h>

void init_blockchain(Blockchain *bc, int difficulty) {
    bc->length = 0;
    bc->difficulty = difficulty;
    
    // Create genesis block
    Block genesis;
    genesis.index = 0;
    genesis.timestamp = time(NULL);
    genesis.transaction_count = 0;
    strcpy(genesis.previous_hash, "0");
    genesis.nonce = 0;
    mine_block(&genesis, difficulty);
    
    bc->blocks[0] = genesis;
    bc->length = 1;
}

int add_block(Blockchain *bc, Block *new_block) {
    if (bc->length >= BLOCKCHAIN_SIZE) {
        return 0;
    }
    
    new_block->index = bc->length;
    new_block->timestamp = time(NULL);
    strcpy(new_block->previous_hash, bc->blocks[bc->length - 1].hash);
    mine_block(new_block, bc->difficulty);
    
    bc->blocks[bc->length] = *new_block;
    bc->length++;
    
    return 1;
}

int is_chain_valid(const Blockchain *bc) {
    for (int i = 1; i < bc->length; i++) {
        const Block *current = &bc->blocks[i];
        const Block *previous = &bc->blocks[i-1];
        
        // Check previous hash reference
        if (strcmp(current->previous_hash, previous->hash) != 0) {
            return 0;
        }
        
        // Check hash validity
        char calculated_hash[65];
        Block temp = *current;
        calculate_hash(&temp, calculated_hash);
        
        if (strcmp(calculated_hash, current->hash) != 0) {
            return 0;
        }
    }
    return 1;
}

void print_blockchain(const Blockchain *bc) {
    printf("Blockchain (Difficulty: %d, Length: %d):\n", bc->difficulty, bc->length);
    for (int i = 0; i < bc->length; i++) {
        print_block(&bc->blocks[i]);
    }
}
