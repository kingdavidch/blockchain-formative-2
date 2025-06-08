#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>

#define MAX_TRANSACTIONS 10
#define MAX_TRANSACTION_LENGTH 100
#define HASH_SIZE 64
#define MAX_BLOCKS 100

// Structure to represent a transaction
typedef struct {
    char data[MAX_TRANSACTION_LENGTH];
} Transaction;

// Structure to represent a block
typedef struct {
    uint32_t index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int num_transactions;
    char previous_hash[HASH_SIZE];
    uint32_t nonce;
    char hash[HASH_SIZE];
} Block;

// Structure to represent the blockchain
typedef struct {
    Block blocks[MAX_BLOCKS];
    int num_blocks;
    int difficulty;  // Number of leading zeros required in hash
} Blockchain;

// Function declarations
void init_blockchain(Blockchain* chain, int initial_difficulty);
void add_transaction(Block* block, const char* transaction_data);
void calculate_hash(Block* block, char* hash);
bool mine_block(Blockchain* chain);
void print_block(const Block* block);
void print_blockchain(const Blockchain* chain);
bool verify_blockchain(const Blockchain* chain);
void adjust_difficulty(Blockchain* chain, int new_difficulty);

#endif // BLOCKCHAIN_H
