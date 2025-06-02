#include "blockchain.h"
#include <stdio.h>
#include <time.h>

int main() {
    Blockchain bc;
    int difficulty = 4; // Start with 4 leading zeros
    
    printf("Initializing blockchain with difficulty %d...\n", difficulty);
    init_blockchain(&bc, difficulty);
    print_blockchain(&bc);
    
    // Mine some blocks with transactions
    for (int i = 0; i < 3; i++) {
        Block new_block;
        new_block.transaction_count = 2;
        snprintf(new_block.transactions[0], MAX_DATA_LENGTH, "Transaction %d-1", i+1);
        snprintf(new_block.transactions[1], MAX_DATA_LENGTH, "Transaction %d-2", i+1);
        new_block.nonce = 0;
        
        clock_t start = clock();
        add_block(&bc, &new_block);
        clock_t end = clock();
        
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Mined block #%d in %.2f seconds\n", i+1, time_spent);
    }
    
    print_blockchain(&bc);
    
    // Validate the chain
    printf("Blockchain is %s\n", is_chain_valid(&bc) ? "valid" : "invalid");
    
    // Adjust difficulty and mine more blocks
    printf("\nAdjusting difficulty to 5...\n");
    bc.difficulty = 5;
    
    Block harder_block;
    harder_block.transaction_count = 1;
    snprintf(harder_block.transactions[0], MAX_DATA_LENGTH, "Harder transaction");
    harder_block.nonce = 0;
    
    clock_t start = clock();
    add_block(&bc, &harder_block);
    clock_t end = clock();
    
    printf("Mined block with higher difficulty in %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    print_block(&bc.blocks[bc.length - 1]);
    
    return 0;
}
