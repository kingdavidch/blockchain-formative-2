#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "blockchain.h"

// Simple SHA-256 implementation (for demonstration purposes)
// In a real implementation, you would use a proper cryptographic library
void sha256(const char* input, char* output) {
    // This is a simplified hash function for demonstration
    // In reality, you should use a proper SHA-256 implementation
    sprintf(output, "%064x", (unsigned int)time(NULL) + rand());
}

void init_blockchain(Blockchain* chain, int initial_difficulty) {
    chain->num_blocks = 0;
    chain->difficulty = initial_difficulty;
    
    // Create genesis block
    Block* genesis = &chain->blocks[0];
    genesis->index = 0;
    genesis->timestamp = time(NULL);
    genesis->num_transactions = 0;
    memset(genesis->previous_hash, '0', HASH_SIZE);
    genesis->previous_hash[HASH_SIZE - 1] = '\0';
    genesis->nonce = 0;
    
    // Calculate genesis block hash
    calculate_hash(genesis, genesis->hash);
    chain->num_blocks = 1;
}

void add_transaction(Block* block, const char* transaction_data) {
    if (block->num_transactions >= MAX_TRANSACTIONS) {
        printf("Error: Block is full\n");
        return;
    }
    
    strncpy(block->transactions[block->num_transactions].data,
            transaction_data,
            MAX_TRANSACTION_LENGTH - 1);
    block->transactions[block->num_transactions].data[MAX_TRANSACTION_LENGTH - 1] = '\0';
    block->num_transactions++;
}

void calculate_hash(Block* block, char* hash) {
    char data_to_hash[1024];
    sprintf(data_to_hash, "%d%ld%d%s",
            block->index,
            block->timestamp,
            block->nonce,
            block->previous_hash);
    
    // Add transactions to the data
    for (int i = 0; i < block->num_transactions; i++) {
        strcat(data_to_hash, block->transactions[i].data);
    }
    
    sha256(data_to_hash, hash);
}

bool verify_hash(const char* hash, int difficulty) {
    for (int i = 0; i < difficulty; i++) {
        if (hash[i] != '0') {
            return false;
        }
    }
    return true;
}

bool mine_block(Blockchain* chain) {
    if (chain->num_blocks >= MAX_BLOCKS) {
        printf("Error: Blockchain is full\n");
        return false;
    }
    
    Block* previous_block = &chain->blocks[chain->num_blocks - 1];
    Block* new_block = &chain->blocks[chain->num_blocks];
    
    // Initialize new block
    new_block->index = previous_block->index + 1;
    new_block->timestamp = time(NULL);
    new_block->num_transactions = 0;
    strcpy(new_block->previous_hash, previous_block->hash);
    
    // Mine the block
    new_block->nonce = 0;
    do {
        calculate_hash(new_block, new_block->hash);
        new_block->nonce++;
    } while (!verify_hash(new_block->hash, chain->difficulty));
    
    chain->num_blocks++;
    return true;
}

void print_block(const Block* block) {
    printf("\nBlock #%d\n", block->index);
    printf("Timestamp: %ld\n", block->timestamp);
    printf("Previous Hash: %s\n", block->previous_hash);
    printf("Hash: %s\n", block->hash);
    printf("Nonce: %d\n", block->nonce);
    printf("Transactions:\n");
    for (int i = 0; i < block->num_transactions; i++) {
        printf("  %s\n", block->transactions[i].data);
    }
    printf("----------------------------------------\n");
}

void print_blockchain(const Blockchain* chain) {
    printf("\nBlockchain Status:\n");
    printf("Number of blocks: %d\n", chain->num_blocks);
    printf("Current difficulty: %d\n", chain->difficulty);
    printf("----------------------------------------\n");
    
    for (int i = 0; i < chain->num_blocks; i++) {
        print_block(&chain->blocks[i]);
    }
}

bool verify_blockchain(const Blockchain* chain) {
    for (int i = 1; i < chain->num_blocks; i++) {
        Block* current = &chain->blocks[i];
        Block* previous = &chain->blocks[i - 1];
        
        // Verify previous hash
        if (strcmp(current->previous_hash, previous->hash) != 0) {
            printf("Blockchain verification failed: Invalid previous hash at block %d\n", i);
            return false;
        }
        
        // Verify current block's hash
        char calculated_hash[HASH_SIZE];
        calculate_hash(current, calculated_hash);
        if (strcmp(current->hash, calculated_hash) != 0) {
            printf("Blockchain verification failed: Invalid hash at block %d\n", i);
            return false;
        }
    }
    return true;
}

void adjust_difficulty(Blockchain* chain, int new_difficulty) {
    if (new_difficulty < 1 || new_difficulty > 64) {
        printf("Error: Difficulty must be between 1 and 64\n");
        return;
    }
    chain->difficulty = new_difficulty;
}
