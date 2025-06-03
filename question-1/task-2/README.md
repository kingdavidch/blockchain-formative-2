#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 10
#define MAX_NAME_LEN 30

// Account structure definition
typedef struct {
    char name[MAX_NAME_LEN];    // Account holder's name
    float balance;              // Current account balance
} Account;

// Global accounts array and counter
Account accounts[MAX_ACCOUNTS];
int account_count = 0;

// Function to create a new account
void create_account(const char* name, float initial_balance) {
    if (account_count < MAX_ACCOUNTS) {
        strcpy(accounts[account_count].name, name);
        accounts[account_count].balance = initial_balance;
        account_count++;
        printf("Account created: %s with balance %.2f\n", name, initial_balance);
    } else {
        printf("ERROR: Maximum number of accounts reached!\n");
    }
}

// Function to find account index by name
int find_account(const char* name) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // Account not found
}

// Function to display all account balances
void display_balances() {
    printf("\n=== ACCOUNT BALANCES ===\n");
    printf("Name\t\t\tBalance\n");
    printf("--------------------------------\n");
    
    for (int i = 0; i < account_count; i++) {
        printf("%-20s\t%.2f\n", accounts[i].name, accounts[i].balance);
    }
    printf("\n");
}

// Function to get account balance by name
float get_balance(const char* name) {
    int index = find_account(name);
    if (index != -1) {
        return accounts[index].balance;
    }
    return -1.0; // Account not found
}

// Main transaction function - transfers funds between accounts
bool transferFunds(const char* sender_name, const char* receiver_name, float amount) {
    printf("\n=== INITIATING TRANSFER ===\n");
    printf("From: %s\n", sender_name);
    printf("To: %s\n", receiver_name);
    printf("Amount: %.2f\n", amount);
    
    // Input validation - check for negative or zero amount
    if (amount <= 0) {
        printf("ERROR: Transfer amount must be positive!\n");
        return false;
    }
    
    // Find sender account
    int sender_index = find_account(sender_name);
    if (sender_index == -1) {
        printf("ERROR: Sender account '%s' not found!\n", sender_name);
        return false;
    }
    
    // Find receiver account
    int receiver_index = find_account(receiver_name);
    if (receiver_index == -1) {
        printf("ERROR: Receiver account '%s' not found!\n", receiver_name);
        return false;
    }
    
    // Check if sender and receiver are the same
    if (sender_index == receiver_index) {
        printf("ERROR: Cannot transfer to the same account!\n");
        return false;
    }
    
    // Validate sender has sufficient balance
    if (accounts[sender_index].balance < amount) {
        printf("ERROR: Insufficient funds!\n");
        printf("Available balance: %.2f, Required: %.2f\n", 
               accounts[sender_index].balance, amount);
        return false;
    }
    
    // Perform the transfer - deduct from sender, add to receiver
    accounts[sender_index].balance -= amount;
    accounts[receiver_index].balance += amount;
    
    printf("SUCCESS: Transfer completed!\n");
    printf("New balance for %s: %.2f\n", sender_name, accounts[sender_index].balance);
    printf("New balance for %s: %.2f\n", receiver_name, accounts[receiver_index].balance);
    
    return true;
}

// Function to display transaction summary
void display_transaction_summary(const char* sender, const char* receiver, float amount, bool success) {
    printf("\n--- Transaction Summary ---\n");
    printf("Sender: %s\n", sender);
    printf("Receiver: %s\n", receiver);
    printf("Amount: %.2f\n", amount);
    printf("Status: %s\n", success ? "SUCCESS" : "FAILED");
    printf("---------------------------\n");
}

// Function to initialize accounts with sample data
void initialize_accounts() {
    printf("=== INITIALIZING ACCOUNT SYSTEM ===\n");
    
    // Create three initial accounts with different balances
    create_account("Alice", 1000.0);
    create_account("Bob", 500.0);
    create_account("Charlie", 750.0);
    
    printf("Account system initialized with 3 accounts\n");
}

// Function to run simulation scenarios
void run_simulation() {
    printf("\n=== RUNNING TRANSACTION SIMULATIONS ===\n");
    
    // Scenario 1: Successful transfer - Alice sends 200 to Bob
    printf("\n>>> SCENARIO 1: Alice sends 200 to Bob\n");
    display_balances();
    bool result1 = transferFunds("Alice", "Bob", 200.0);
    display_transaction_summary("Alice", "Bob", 200.0, result1);
    display_balances();
    
    // Scenario 2: Successful transfer - Bob sends 150 to Charlie
    printf("\n>>> SCENARIO 2: Bob sends 150 to Charlie\n");
    bool result2 = transferFunds("Bob", "Charlie", 150.0);
    display_transaction_summary("Bob", "Charlie", 150.0, result2);
    display_balances();
    
    // Scenario 3: Failed transfer - Charlie tries to send more than he has
    printf("\n>>> SCENARIO 3: Charlie tries to send 1000 to Alice (should fail)\n");
    bool result3 = transferFunds("Charlie", "Alice", 1000.0);
    display_transaction_summary("Charlie", "Alice", 1000.0, result3);
    display_balances();
    
    // Scenario 4: Failed transfer - Invalid amount (negative)
    printf("\n>>> SCENARIO 4: Alice tries to send -50 to Bob (should fail)\n");
    bool result4 = transferFunds("Alice", "Bob", -50.0);
    display_transaction_summary("Alice", "Bob", -50.0, result4);
    display_balances();
    
    // Scenario 5: Failed transfer - Non-existent account
    printf("\n>>> SCENARIO 5: Transfer to non-existent account (should fail)\n");
    bool result5 = transferFunds("Alice", "David", 100.0);
    display_transaction_summary("Alice", "David", 100.0, result5);
    display_balances();
    
    // Scenario 6: Successful transfer - Charlie sends remaining balance to Alice
    printf("\n>>> SCENARIO 6: Charlie sends 50 to Alice\n");
    bool result6 = transferFunds("Charlie", "Alice", 50.0);
    display_transaction_summary("Charlie", "Alice", 50.0, result6);
    display_balances();
}

// Function to display final system state
void display_final_state() {
    printf("\n=== FINAL SYSTEM STATE ===\n");
    printf("Total accounts: %d\n", account_count);
    
    float total_balance = 0.0;
    for (int i = 0; i < account_count; i++) {
        total_balance += accounts[i].balance;
    }
    
    printf("Total system balance: %.2f\n", total_balance);
    display_balances();
    
    printf("=== SIMULATION COMPLETE ===\n");
}

int main() {
    printf("Account/Balance-Based Transaction Simulation\n");
    printf("==========================================\n");
    
    // Initialize the account system
    initialize_accounts();
    
    // Show initial state
    printf("\nInitial state:\n");
    display_balances();
    
    // Run various transaction scenarios
    run_simulation();
    
    // Display final system state
    display_final_state();
    
    return 0;
}
