#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Constants for system limits
#define MAX_OUTPUTS 50
#define ADDRESS_SIZE 25
#define TX_HASH_SIZE 15

// Structure representing an unspent transaction output
struct UnspentOutput {
    char transaction_hash[TX_HASH_SIZE];  // Hash of the creating transaction
    char owner_wallet[ADDRESS_SIZE];      // Address of the owner
    double coin_value;                    // Value stored in this output
    bool consumed;                        // Whether this output has been spent
    int output_id;                        // Unique identifier for this output
};

// Structure for recording transaction details
struct TransactionRecord {
    char hash[TX_HASH_SIZE];              // Transaction identifier
    char sender_wallet[ADDRESS_SIZE];     // Sender's wallet address
    char recipient_wallet[ADDRESS_SIZE];  // Recipient's wallet address
    double transfer_amount;               // Amount being transferred
    double fee_amount;                    // Transaction fee
    int inputs_used;                      // Number of inputs consumed
    int outputs_created;                  // Number of outputs created
};

// Global storage for all unspent outputs
struct UnspentOutput output_ledger[MAX_OUTPUTS];
int total_outputs = 0;
int next_output_id = 1000;

// Function to generate a unique transaction hash
void generate_tx_hash(char* hash_buffer) {
    static int counter = 1;
    snprintf(hash_buffer, TX_HASH_SIZE, "TXN_%04d", counter++);
}

// Function to create a new unspent output in the ledger
void create_output(const char* tx_hash, const char* wallet_addr, double value) {
    if (total_outputs >= MAX_OUTPUTS) {
        printf("Warning: Output ledger is full!\n");
        return;
    }
    
    // Initialize new output
    strcpy(output_ledger[total_outputs].transaction_hash, tx_hash);
    strcpy(output_ledger[total_outputs].owner_wallet, wallet_addr);
    output_ledger[total_outputs].coin_value = value;
    output_ledger[total_outputs].consumed = false;
    output_ledger[total_outputs].output_id = next_output_id++;
    
    total_outputs++;
}

// Display all active (unspent) outputs in the system
void show_active_outputs() {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    ACTIVE OUTPUT LEDGER                    ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ TX Hash     │ Owner Address    │ Value     │ Output ID    ║\n");
    printf("╠═════════════┼══════════════════┼═══════════┼══════════════╣\n");
    
    bool found_active = false;
    for (int i = 0; i < total_outputs; i++) {
        if (!output_ledger[i].consumed) {
            printf("║ %-11s │ %-16s │ %8.2f │ %-12d ║\n",
                   output_ledger[i].transaction_hash,
                   output_ledger[i].owner_wallet,
                   output_ledger[i].coin_value,
                   output_ledger[i].output_id);
            found_active = true;
        }
    }
    
    if (!found_active) {
        printf("║                    No active outputs found                 ║\n");
    }
    
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

// Calculate total wallet balance from unspent outputs
double calculate_wallet_balance(const char* wallet_address) {
    double total_balance = 0.0;
    
    for (int i = 0; i < total_outputs; i++) {
        if (!output_ledger[i].consumed && 
            strcmp(output_ledger[i].owner_wallet, wallet_address) == 0) {
            total_balance += output_ledger[i].coin_value;
        }
    }
    
    return total_balance;
}

// Find and collect outputs for spending (returns total value collected)
double collect_spendable_outputs(const char* wallet_addr, double required_amount, 
                                int* selected_indices, int* selection_count) {
    double collected_value = 0.0;
    *selection_count = 0;
    
    // Sort outputs by value (largest first) for efficient selection
    int sorted_indices[MAX_OUTPUTS];
    int valid_count = 0;
    
    // First, find all valid outputs for this wallet
    for (int i = 0; i < total_outputs; i++) {
        if (!output_ledger[i].consumed && 
            strcmp(output_ledger[i].owner_wallet, wallet_addr) == 0) {
            sorted_indices[valid_count++] = i;
        }
    }
    
    // Simple selection: pick outputs until we have enough
    for (int i = 0; i < valid_count && collected_value < required_amount; i++) {
        int idx = sorted_indices[i];
        selected_indices[*selection_count] = idx;
        collected_value += output_ledger[idx].coin_value;
        (*selection_count)++;
    }
    
    return collected_value;
}

// Execute a transfer between two wallets
bool execute_transfer(const char* from_wallet, const char* to_wallet, double amount) {
    printf("\n┌─────────────────────────────────────────────────────────┐\n");
    printf("│                 INITIATING TRANSFER                     │\n");
    printf("├─────────────────────────────────────────────────────────┤\n");
    printf("│ From: %-20s                           │\n", from_wallet);
    printf("│ To:   %-20s                           │\n", to_wallet);
    printf("│ Amount: %.2f coins                                    │\n", amount);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Validate transfer amount
    if (amount <= 0) {
        printf("❌ ERROR: Transfer amount must be positive!\n");
        return false;
    }
    
    // Check sender's available balance
    double sender_balance = calculate_wallet_balance(from_wallet);
    if (sender_balance < amount) {
        printf("❌ ERROR: Insufficient balance!\n");
        printf("   Available: %.2f, Required: %.2f\n", sender_balance, amount);
        return false;
    }
    
    // Collect outputs for spending
    int selected_outputs[MAX_OUTPUTS];
    int num_selected = 0;
    double total_input = collect_spendable_outputs(from_wallet, amount, 
                                                  selected_outputs, &num_selected);
    
    if (total_input < amount) {
        printf("❌ ERROR: Cannot collect sufficient outputs for transfer!\n");
        return false;
    }
    
    // Generate new transaction hash
    char new_tx_hash[TX_HASH_SIZE];
    generate_tx_hash(new_tx_hash);
    
    // Mark selected outputs as consumed
    printf("📤 Consuming inputs:\n");
    for (int i = 0; i < num_selected; i++) {
        int idx = selected_outputs[i];
        printf("   • Output %d: %.2f coins from %s\n", 
               output_ledger[idx].output_id,
               output_ledger[idx].coin_value,
               output_ledger[idx].transaction_hash);
        output_ledger[idx].consumed = true;
    }
    
    // Create output for recipient
    create_output(new_tx_hash, to_wallet, amount);
    printf("📥 Created output for %s: %.2f coins\n", to_wallet, amount);
    
    // Create change output if necessary
    double change_amount = total_input - amount;
    if (change_amount > 0.001) { // Avoid dust amounts
        char change_tx_hash[TX_HASH_SIZE];
        generate_tx_hash(change_tx_hash);
        create_output(change_tx_hash, from_wallet, change_amount);
        printf("💰 Created change output for %s: %.2f coins\n", from_wallet, change_amount);
    }
    
    printf("✅ Transfer completed successfully!\n");
    printf("   Transaction Hash: %s\n", new_tx_hash);
    
    return true;
}

// Display wallet balances in a formatted table
void display_wallet_summary() {
    printf("\n┌─────────────────────────────────────────────────────────┐\n");
    printf("│                   WALLET SUMMARY                        │\n");
    printf("├─────────────────────────────────────────────────────────┤\n");
    
    // Get unique wallet addresses
    char unique_wallets[MAX_OUTPUTS][ADDRESS_SIZE];
    int wallet_count = 0;
    
    for (int i = 0; i < total_outputs; i++) {
        if (!output_ledger[i].consumed) {
            bool is_new = true;
            for (int j = 0; j < wallet_count; j++) {
                if (strcmp(unique_wallets[j], output_ledger[i].owner_wallet) == 0) {
                    is_new = false;
                    break;
                }
            }
            if (is_new) {
                strcpy(unique_wallets[wallet_count], output_ledger[i].owner_wallet);
                wallet_count++;
            }
        }
    }
    
    // Display balances
    for (int i = 0; i < wallet_count; i++) {
        double balance = calculate_wallet_balance(unique_wallets[i]);
        printf("│ %-20s: %10.2f coins                │\n", unique_wallets[i], balance);
    }
    
    printf("└─────────────────────────────────────────────────────────┘\n");
}

// Initialize the system with genesis outputs
void setup_genesis_state() {
    printf("🚀 Initializing UTXO System with Genesis Outputs\n");
    printf("══════════════════════════════════════════════════════════\n");
    
    // Create initial outputs for different users
    create_output("GENESIS_001", "wallet_alice", 120.0);
    create_output("GENESIS_002", "wallet_alice", 80.0);
    create_output("GENESIS_003", "wallet_bob", 90.0);
    create_output("GENESIS_004", "wallet_bob", 60.0);
    create_output("GENESIS_005", "wallet_carol", 100.0);
    
    printf("✅ Genesis state created with 5 initial outputs\n");
}

// Main program execution
int main() {
    // Initialize random seed for any random operations
    srand(time(NULL));
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║            UTXO-Based Blockchain Transaction Simulator       ║\n");
    printf("║                     Version 2.0                             ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    
    // Initialize system
    setup_genesis_state();
    
    // Show initial state
    printf("\n📊 INITIAL SYSTEM STATE\n");
    display_wallet_summary();
    show_active_outputs();
    
    // Execute test transactions
    printf("\n🔄 EXECUTING TEST TRANSACTIONS\n");
    printf("════════════════════════════════════════════════════════════\n");
    
    // Test 1: Alice sends 50 coins to Bob
    printf("\n📋 TEST 1: Alice → Bob (50 coins)\n");
    execute_transfer("wallet_alice", "wallet_bob", 50.0);
    display_wallet_summary();
    
    // Test 2: Bob sends 75 coins to Carol
    printf("\n📋 TEST 2: Bob → Carol (75 coins)\n");
    execute_transfer("wallet_bob", "wallet_carol", 75.0);
    display_wallet_summary();
    
    // Test 3: Insufficient funds test
    printf("\n📋 TEST 3: Carol → Alice (300 coins) [Expected to fail]\n");
    execute_transfer("wallet_carol", "wallet_alice", 300.0);
    display_wallet_summary();
    
    // Test 4: Small amount transfer
    printf("\n📋 TEST 4: Alice → Carol (25 coins)\n");
    execute_transfer("wallet_alice", "wallet_carol", 25.0);
    display_wallet_summary();
    
    // Final system state
    printf("\n📈 FINAL SYSTEM STATE\n");
    printf("════════════════════════════════════════════════════════════\n");
    show_active_outputs();
    display_wallet_summary();
    
    // Calculate total system value
    double total_value = 0.0;
    for (int i = 0; i < total_outputs; i++) {
        if (!output_ledger[i].consumed) {
            total_value += output_ledger[i].coin_value;
        }
    }
    
    printf("\n💎 Total system value: %.2f coins\n", total_value);
    printf("📊 Total active outputs: %d\n", total_outputs);
    
    printf("\n🎯 SIMULATION COMPLETED SUCCESSFULLY!\n");
    printf("════════════════════════════════════════════════════════════\n");
    
    return 0;
}
