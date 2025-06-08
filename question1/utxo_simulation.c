#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_UTXOS 100
#define MAX_ADDRESS_LENGTH 50
#define MAX_TXID_LENGTH 64

// Structure to represent a UTXO
typedef struct {
    char txid[MAX_TXID_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    float amount;
    bool is_spent;
} UTXO;

// Structure to represent a transaction
typedef struct {
    char txid[MAX_TXID_LENGTH];
    UTXO* inputs[MAX_UTXOS];
    int num_inputs;
    char receiver[MAX_ADDRESS_LENGTH];
    float amount;
} Transaction;

// Global UTXO pool
UTXO utxo_pool[MAX_UTXOS];
int num_utxos = 0;

// Function to generate a random transaction ID
void generate_txid(char* txid) {
    for (int i = 0; i < MAX_TXID_LENGTH - 1; i++) {
        txid[i] = '0' + (rand() % 10);
    }
    txid[MAX_TXID_LENGTH - 1] = '\0';
}

// Function to create a new UTXO
UTXO* create_utxo(const char* address, float amount) {
    if (num_utxos >= MAX_UTXOS) {
        printf("Error: UTXO pool is full\n");
        return NULL;
    }

    UTXO* utxo = &utxo_pool[num_utxos++];
    generate_txid(utxo->txid);
    strncpy(utxo->address, address, MAX_ADDRESS_LENGTH - 1);
    utxo->amount = amount;
    utxo->is_spent = false;
    return utxo;
}

// Function to find available UTXOs for an address
int find_available_utxos(const char* address, UTXO** available_utxos) {
    int count = 0;
    for (int i = 0; i < num_utxos; i++) {
        if (!utxo_pool[i].is_spent && strcmp(utxo_pool[i].address, address) == 0) {
            available_utxos[count++] = &utxo_pool[i];
        }
    }
    return count;
}

// Function to create and process a transaction
bool create_transaction(const char* sender, const char* receiver, float amount) {
    UTXO* available_utxos[MAX_UTXOS];
    int num_available = find_available_utxos(sender, available_utxos);
    
    if (num_available == 0) {
        printf("Error: No available UTXOs for sender %s\n", sender);
        return false;
    }

    // Calculate total available amount
    float total_available = 0;
    for (int i = 0; i < num_available; i++) {
        total_available += available_utxos[i]->amount;
    }

    if (total_available < amount) {
        printf("Error: Insufficient funds. Available: %.2f, Required: %.2f\n", 
               total_available, amount);
        return false;
    }

    // Create transaction
    Transaction tx;
    generate_txid(tx.txid);
    tx.num_inputs = 0;
    strncpy(tx.receiver, receiver, MAX_ADDRESS_LENGTH - 1);
    tx.amount = amount;

    // Select UTXOs to spend
    float remaining = amount;
    for (int i = 0; i < num_available && remaining > 0; i++) {
        tx.inputs[tx.num_inputs++] = available_utxos[i];
        remaining -= available_utxos[i]->amount;
        available_utxos[i]->is_spent = true;
    }

    // Create output UTXOs
    create_utxo(receiver, amount);
    if (remaining < 0) {
        create_utxo(sender, -remaining); // Create change UTXO
    }

    printf("Transaction successful!\n");
    printf("Transaction ID: %s\n", tx.txid);
    printf("Amount: %.2f\n", amount);
    printf("From: %s\n", sender);
    printf("To: %s\n", receiver);
    return true;
}

// Function to print all available UTXOs
void print_available_utxos() {
    printf("\nAvailable UTXOs:\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < num_utxos; i++) {
        if (!utxo_pool[i].is_spent) {
            printf("TXID: %s\n", utxo_pool[i].txid);
            printf("Address: %s\n", utxo_pool[i].address);
            printf("Amount: %.2f\n", utxo_pool[i].amount);
            printf("----------------------------------------\n");
        }
    }
}

int main() {
    // Initialize random seed
    srand(time(NULL));

    // Create initial UTXOs for two users
    create_utxo("King", 100.0);
    create_utxo("Jack", 50.0);

    printf("Initial UTXO state:\n");
    print_available_utxos();

    // Simulate some transactions
    printf("\nSimulating transactions...\n");
    
    // King sends 30 to Jack
    create_transaction("King", "Jack", 30.0);
    print_available_utxos();

    // Jack sends 20 back to King
    create_transaction("Jack", "King", 20.0);
    print_available_utxos();

    return 0;
}
