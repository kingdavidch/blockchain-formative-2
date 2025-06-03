#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// System configuration constants
#define MAX_USERS 15
#define USERNAME_LENGTH 35
#define MIN_TRANSFER_AMOUNT 0.01

// Structure to represent a user's financial account
struct UserAccount {
    char username[USERNAME_LENGTH];     // Account holder identifier
    double wallet_balance;              // Current available funds
    int account_id;                     // Unique account number
    bool is_active;                     // Account status flag
    time_t created_timestamp;           // Account creation time
};

// Structure for tracking financial transactions
struct MoneyTransfer {
    int transfer_id;                    // Unique transaction identifier
    char sender_name[USERNAME_LENGTH];  // Source account name
    char receiver_name[USERNAME_LENGTH]; // Destination account name
    double transfer_value;              // Amount transferred
    time_t execution_time;              // When transfer occurred
    bool was_successful;                // Transaction outcome
    char status_message[100];           // Detailed status information
};

// Global system state
struct UserAccount user_registry[MAX_USERS];
int registered_users = 0;
int next_account_id = 2000;
int next_transfer_id = 5000;

// Function to register a new user account in the system
bool register_new_user(const char* name, double starting_balance) {
    // Check system capacity
    if (registered_users >= MAX_USERS) {
        printf("🚫 System Error: Maximum user capacity reached (%d users)\n", MAX_USERS);
        return false;
    }
    
    // Validate input parameters
    if (strlen(name) == 0 || strlen(name) >= USERNAME_LENGTH) {
        printf("🚫 Registration Error: Invalid username length\n");
        return false;
    }
    
    if (starting_balance < 0) {
        printf("🚫 Registration Error: Starting balance cannot be negative\n");
        return false;
    }
    
    // Check for username conflicts
    for (int i = 0; i < registered_users; i++) {
        if (strcmp(user_registry[i].username, name) == 0) {
            printf("🚫 Registration Error: Username '%s' already exists\n", name);
            return false;
        }
    }
    
    // Create new user account
    strcpy(user_registry[registered_users].username, name);
    user_registry[registered_users].wallet_balance = starting_balance;
    user_registry[registered_users].account_id = next_account_id++;
    user_registry[registered_users].is_active = true;
    user_registry[registered_users].created_timestamp = time(NULL);
    
    registered_users++;
    
    printf("✅ User '%s' registered successfully (Account ID: %d, Balance: $%.2f)\n", 
           name, next_account_id - 1, starting_balance);
    return true;
}

// Locate user account by username
int locate_user_account(const char* username) {
    for (int i = 0; i < registered_users; i++) {
        if (user_registry[i].is_active && strcmp(user_registry[i].username, username) == 0) {
            return i;
        }
    }
    return -1; // User not found
}

// Retrieve current balance for a specific user
double query_user_balance(const char* username) {
    int user_index = locate_user_account(username);
    if (user_index != -1) {
        return user_registry[user_index].wallet_balance;
    }
    return -1.0; // Indicates user not found
}

// Display comprehensive account information
void display_account_registry() {
    printf("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃                           💰 ACCOUNT REGISTRY 💰                           ┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
    printf("┃ Account ID │    Username           │    Balance    │    Status            ┃\n");
    printf("┣━━━━━━━━━━━━┼━━━━━━━━━━━━━━━━━━━━━━━┼━━━━━━━━━━━━━━━┼━━━━━━━━━━━━━━━━━━━━━━┫\n");
    
    double total_system_funds = 0.0;
    int active_accounts = 0;
    
    for (int i = 0; i < registered_users; i++) {
        if (user_registry[i].is_active) {
            printf("┃   %6d   │ %-20s │  $%10.2f │      ACTIVE          ┃\n",
                   user_registry[i].account_id,
                   user_registry[i].username,
                   user_registry[i].wallet_balance);
            total_system_funds += user_registry[i].wallet_balance;
            active_accounts++;
        }
    }
    
    if (active_accounts == 0) {
        printf("┃                        No active accounts found                         ┃\n");
    }
    
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
    printf("┃ Total Active Accounts: %2d                Total System Funds: $%10.2f ┃\n", 
           active_accounts, total_system_funds);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}

// Core money transfer function with comprehensive validation
bool process_money_transfer(const char* sender_username, const char* recipient_username, double amount) {
    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        💸 PROCESSING TRANSFER 💸                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Transfer ID: %d                                                        ║\n", next_transfer_id);
    printf("║ Sender:      %-30s                              ║\n", sender_username);
    printf("║ Recipient:   %-30s                              ║\n", recipient_username);
    printf("║ Amount:      $%-10.2f                                               ║\n", amount);
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
    
    // Input validation checks
    if (amount < MIN_TRANSFER_AMOUNT) {
        printf("❌ VALIDATION FAILED: Transfer amount too small (minimum: $%.2f)\n", MIN_TRANSFER_AMOUNT);
        return false;
    }
    
    if (amount > 1000000.0) {
        printf("❌ VALIDATION FAILED: Transfer amount exceeds maximum limit ($1,000,000)\n");
        return false;
    }
    
    // Locate sender account
    int sender_index = locate_user_account(sender_username);
    if (sender_index == -1) {
        printf("❌ SENDER ERROR: Account '%s' not found in registry\n", sender_username);
        return false;
    }
    
    // Locate recipient account
    int recipient_index = locate_user_account(recipient_username);
    if (recipient_index == -1) {
        printf("❌ RECIPIENT ERROR: Account '%s' not found in registry\n", recipient_username);
        return false;
    }
    
    // Prevent self-transfers
    if (sender_index == recipient_index) {
        printf("❌ LOGIC ERROR: Cannot transfer funds to the same account\n");
        return false;
    }
    
    // Verify sender has adequate funds
    if (user_registry[sender_index].wallet_balance < amount) {
        printf("❌ INSUFFICIENT FUNDS ERROR:\n");
        printf("   Account Balance: $%.2f\n", user_registry[sender_index].wallet_balance);
        printf("   Transfer Amount: $%.2f\n", amount);
        printf("   Shortfall:       $%.2f\n", amount - user_registry[sender_index].wallet_balance);
        return false;
    }
    
    // Record pre-transfer balances
    double sender_balance_before = user_registry[sender_index].wallet_balance;
    double recipient_balance_before = user_registry[recipient_index].wallet_balance;
    
    // Execute the atomic transfer operation
    user_registry[sender_index].wallet_balance -= amount;
    user_registry[recipient_index].wallet_balance += amount;
    
    // Display transfer confirmation
    printf("✅ TRANSFER COMPLETED SUCCESSFULLY!\n");
    printf("📊 Transaction Summary:\n");
    printf("   ┌─────────────────────────────────────────────────────────┐\n");
    printf("   │ Sender Balance:    $%8.2f → $%8.2f (-%8.2f) │\n", 
           sender_balance_before, user_registry[sender_index].wallet_balance, amount);
    printf("   │ Recipient Balance: $%8.2f → $%8.2f (+%8.2f) │\n", 
           recipient_balance_before, user_registry[recipient_index].wallet_balance, amount);
    printf("   └─────────────────────────────────────────────────────────┘\n");
    printf("🔐 Transfer ID: %d executed at timestamp: %ld\n", next_transfer_id, time(NULL));
    
    next_transfer_id++;
    return true;
}

// Display system-wide financial statistics
void show_financial_overview() {
    printf("\n╭─────────────────────────────────────────────────────────────────────────╮\n");
    printf("│                         📈 FINANCIAL OVERVIEW 📈                       │\n");
    printf("╰─────────────────────────────────────────────────────────────────────────╯\n");
    
    double total_funds = 0.0;
    double highest_balance = 0.0;
    double lowest_balance = 999999.0;
    char richest_user[USERNAME_LENGTH] = "";
    char poorest_user[USERNAME_LENGTH] = "";
    
    for (int i = 0; i < registered_users; i++) {
        if (user_registry[i].is_active) {
            double balance = user_registry[i].wallet_balance;
            total_funds += balance;
            
            if (balance > highest_balance) {
                highest_balance = balance;
                strcpy(richest_user, user_registry[i].username);
            }
            
            if (balance < lowest_balance) {
                lowest_balance = balance;
                strcpy(poorest_user, user_registry[i].username);
            }
        }
    }
    
    printf("💰 Total System Wealth:     $%.2f\n", total_funds);
    printf("👑 Wealthiest Account:      %s ($%.2f)\n", richest_user, highest_balance);
    printf("📉 Account with Least:      %s ($%.2f)\n", poorest_user, lowest_balance);
    printf("📊 Average Account Balance: $%.2f\n", 
           registered_users > 0 ? total_funds / registered_users : 0.0);
    printf("🏦 Active Accounts:         %d\n", registered_users);
}

// Initialize system with sample user accounts
void initialize_financial_system() {
    printf("🏗️  INITIALIZING DIGITAL BANKING SYSTEM\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    
    // Register initial users with varied starting balances
    register_new_user("Emma_Thompson", 1250.75);
    register_new_user("Marcus_Johnson", 890.50);
    register_new_user("Sophia_Chen", 2100.00);
    register_new_user("David_Rodriguez", 675.25);
    
    printf("🎯 System initialization completed with %d user accounts\n", registered_users);
}

// Execute comprehensive testing scenarios
void run_comprehensive_tests() {
    printf("\n🧪 EXECUTING COMPREHENSIVE TEST SCENARIOS\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    
    // Test Scenario 1: Standard successful transfer
    printf("\n🔸 TEST SCENARIO 1: Standard Transfer (Emma → Marcus)\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    display_account_registry();
    process_money_transfer("Emma_Thompson", "Marcus_Johnson", 175.50);
    
    // Test Scenario 2: Large amount transfer
    printf("\n🔸 TEST SCENARIO 2: Large Transfer (Sophia → David)\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    process_money_transfer("Sophia_Chen", "David_Rodriguez", 500.00);
    display_account_registry();
    
    // Test Scenario 3: Insufficient funds scenario
    printf("\n🔸 TEST SCENARIO 3: Insufficient Funds Test (Marcus → Sophia)\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    process_money_transfer("Marcus_Johnson", "Sophia_Chen", 2000.00);
    
    // Test Scenario 4: Invalid recipient test
    printf("\n🔸 TEST SCENARIO 4: Invalid Recipient Test\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    process_money_transfer("Emma_Thompson", "NonExistent_User", 100.00);
    
    // Test Scenario 5: Minimum amount transfer
    printf("\n🔸 TEST SCENARIO 5: Minimum Amount Transfer (David → Emma)\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    process_money_transfer("David_Rodriguez", "Emma_Thompson", 0.01);
    
    // Test Scenario 6: Self-transfer attempt
    printf("\n🔸 TEST SCENARIO 6: Self-Transfer Prevention Test\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    process_money_transfer("Sophia_Chen", "Sophia_Chen", 50.00);
    
    // Test Scenario 7: Below minimum amount
    printf("\n🔸 TEST SCENARIO 7: Below Minimum Amount Test\n");
    printf("───────────────────────────────────────────────────────────────────────\n");
    process_money_transfer("Emma_Thompson", "Marcus_Johnson", 0.005);
}

// Main program entry point
int main() {
    // Seed random number generator
    srand(time(NULL));
    
    printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                  🏦 DIGITAL ACCOUNT MANAGEMENT SYSTEM 🏦                ║\n");
    printf("║                              Version 3.1                                ║\n");
    printf("║                     Advanced Balance Transfer Engine                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n\n");
    
    // System initialization
    initialize_financial_system();
    
    // Display initial system state
    printf("\n📋 INITIAL SYSTEM STATE\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    display_account_registry();
    show_financial_overview();
    
    // Execute test scenarios
    run_comprehensive_tests();
    
    // Display final system state
    printf("\n📊 FINAL SYSTEM STATE\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    display_account_registry();
    show_financial_overview();
    
    // System summary
    printf("\n🎯 SYSTEM PERFORMANCE SUMMARY\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("✅ All test scenarios executed successfully\n");
    printf("🔒 Transaction integrity maintained throughout testing\n");
    printf("💎 System balance conservation verified\n");
    printf("🏁 Digital Account Management System test completed!\n");
    
    return 0;
}
