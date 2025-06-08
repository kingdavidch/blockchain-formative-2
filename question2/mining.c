#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "blockchain.h"

// Function to measure mining time
double measure_mining_time(Blockchain* chain) {
    clock_t start = clock();
    bool success = mine_block(chain);
    clock_t end = clock();
    
    if (!success) {
        return -1.0;
    }
    
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Function to demonstrate mining with different difficulties
void demonstrate_mining_difficulty() {
    Blockchain chain;
    int difficulties[] = {1, 2, 3, 4};
    int num_difficulties = sizeof(difficulties) / sizeof(difficulties[0]);
    
    printf("Mining Difficulty Demonstration\n");
    printf("==============================\n\n");
    
    for (int i = 0; i < num_difficulties; i++) {
        // Initialize blockchain with current difficulty
        init_blockchain(&chain, difficulties[i]);
        
        // Add some transactions
        add_transaction(&chain.blocks[0], "Transaction 1: Alice sends 10 BTC to Bob");
        add_transaction(&chain.blocks[0], "Transaction 2: Bob sends 5 BTC to Charlie");
        
        printf("Testing difficulty level: %d\n", difficulties[i]);
        printf("----------------------------------------\n");
        
        // Mine a block and measure time
        double mining_time = measure_mining_time(&chain);
        
        if (mining_time >= 0) {
            printf("Mining successful!\n");
            printf("Time taken: %.2f seconds\n", mining_time);
            printf("Block hash: %s\n", chain.blocks[1].hash);
            printf("Nonce found: %d\n", chain.blocks[1].nonce);
        } else {
            printf("Mining failed!\n");
        }
        
        printf("\n");
    }
}

int main() {
    // Initialize random seed
    srand(time(NULL));
    
    // Demonstrate mining with different difficulties
    demonstrate_mining_difficulty();
    
    return 0;
}
