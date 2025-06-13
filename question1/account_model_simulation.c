#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 10
#define MAX_NAME_LENGTH 50

// Structure to represent an account
typedef struct {
    char name[MAX_NAME_LENGTH];
    float balance;
} Account;

// Global array to store accounts
Account accounts[MAX_ACCOUNTS];
int num_accounts = 0;

// Function to create a new account
bool create_account(const char* name, float initial_balance) {
    if (num_accounts >= MAX_ACCOUNTS) {
        printf("Error: Maximum number of accounts reached\n");
        return false;
    }

    // Check if account name already exists
    for (int i = 0; i < num_accounts; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            printf("Error: Account with name '%s' already exists\n", name);
            return false;
        }
    }

    // Create new account
    strncpy(accounts[num_accounts].name, name, MAX_NAME_LENGTH - 1);
    accounts[num_accounts].name[MAX_NAME_LENGTH - 1] = '\0';
    accounts[num_accounts].balance = initial_balance;
    num_accounts++;

    printf("Account created successfully for %s with balance %.2f\n", name, initial_balance);
    return true;
}

// Function to find an account by name
int find_account(const char* name) {
    for (int i = 0; i < num_accounts; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to transfer funds between accounts
bool transfer_funds(const char* sender_name, const char* receiver_name, float amount) {
    // Find sender and receiver accounts
    int sender_index = find_account(sender_name);
    int receiver_index = find_account(receiver_name);

    // Validate accounts exist
    if (sender_index == -1) {
        printf("Error: Sender account '%s' not found\n", sender_name);
        return false;
    }
    if (receiver_index == -1) {
        printf("Error: Receiver account '%s' not found\n", receiver_name);
        return false;
    }

    // Validate sufficient balance
    if (accounts[sender_index].balance < amount) {
        printf("Error: Insufficient funds. Available: %.2f, Required: %.2f\n",
               accounts[sender_index].balance, amount);
        return false;
    }

    // Perform the transfer
    accounts[sender_index].balance -= amount;
    accounts[receiver_index].balance += amount;

    printf("Transfer successful!\n");
    printf("Amount: %.2f\n", amount);
    printf("From: %s (New balance: %.2f)\n", sender_name, accounts[sender_index].balance);
    printf("To: %s (New balance: %.2f)\n", receiver_name, accounts[receiver_index].balance);
    return true;
}

// Function to display all account balances
void display_balances() {
    printf("\nCurrent Account Balances:\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < num_accounts; i++) {
        printf("Account: %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("----------------------------------------\n");
    }
}

int main() {
    // Initialize three accounts with different balances
    create_account("King", 1000.0);
    create_account("Jack", 500.0);
    create_account("Queen", 750.0);

    // Display initial balances
    printf("\nInitial account balances:\n");
    display_balances();

    // Simulate some transactions
    printf("\nSimulating transactions...\n");
    
    // King sends 200 to Jack
    transfer_funds("King", "Jack", 200.0);
    display_balances();

    // Jack sends 100 to Queen
    transfer_funds("Jack", "Queen", 100.0);
    display_balances();

    // Queen sends 150 to King
    transfer_funds("Queen", "King", 150.0);
    display_balances();

    return 0;
} 