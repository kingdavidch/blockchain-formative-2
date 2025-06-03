Account/Balance Model Simulation Program
=======================================

OVERVIEW:
This C program simulates an Account/Balance-based transaction system similar 
to Ethereum's model. Unlike UTXO systems, this model maintains account balances 
that are directly updated during transactions.

CODE STRUCTURE AND FUNCTIONS:

1. Account Structure:
   - Contains account name (string) and current balance (float)
   - Simple and straightforward representation of user accounts

2. create_account(name, initial_balance):
   - Creates new accounts in the global accounts array
   - Validates maximum account limit
   - Initializes account with given name and balance

3. find_account(name):
   - Searches for account by name in the accounts array
   - Returns account index if found, -1 if not found
   - Used internally by other functions for account lookup

4. display_balances():
   - Prints formatted table showing all account names and balances
   - Provides clear overview of current system state
   - Called before and after transactions for comparison

5. get_balance(name):
   - Returns current balance for a specific account
   - Returns -1.0 if account doesn't exist
   - Utility function for balance queries

6. transferFunds(sender_name, receiver_name, amount):
   - Main transaction function that performs fund transfers
   - Comprehensive validation including:
     * Positive amount check
     * Sender account existence
     * Receiver account existence
     * Same account transfer prevention
     * Sufficient balance verification
   - Atomic operation: deducts from sender, adds to receiver
   - Returns boolean indicating success/failure

7. display_transaction_summary():
   - Shows formatted summary of each transaction attempt
   - Includes sender, receiver, amount, and status
   - Helps track transaction history and outcomes

8. initialize_accounts():
   - Sets up initial system state with three accounts
   - Alice: 1000.0, Bob: 500.0, Charlie: 750.0
   - Provides starting point for simulation scenarios

9. run_simulation():
   - Executes predefined transaction scenarios
   - Tests various success and failure cases
   - Demonstrates system behavior under different conditions

LOGIC BEHIND THE SYSTEM:

Account Model Benefits:
- Simple balance tracking (no UTXO management)
- Direct account-to-account transfers
- Easy balance queries and updates
- Lower computational overhead

Transaction Process:
1. Validate transaction parameters (amount, accounts)
2. Check sender has sufficient balance
3. Simultaneously deduct from sender and add to receiver
4. Update account balances in-place
5. Provide transaction confirmation

Error Handling:
- Prevents negative transfers
- Validates account existence
- Ensures sufficient funds
- Prevents self-transfers
- Maintains data integrity

COMPILATION:
gcc -o account_model_simulation account_model_simulation.c

EXECUTION:
./account_model_simulation

EXPECTED BEHAVIOR:

Initial State:
Alice: 1000.00
Bob: 500.00
Charlie: 750.00
Total: 2250.00

Scenario 1 - Alice sends 200 to Bob:
✓ SUCCESS: Alice: 800.00, Bob: 700.00

Scenario 2 - Bob sends 150 to Charlie:
✓ SUCCESS: Bob: 550.00, Charlie: 900.00

Scenario 3 - Charlie tries to send 1000 to Alice:
✗ FAILED: Insufficient funds (Charlie has 900.00)

Scenario 4 - Alice tries to send -50 to Bob:
✗ FAILED: Negative amount not allowed

Scenario 5 - Transfer to non-existent account:
✗ FAILED: Receiver account 'David' not found

Scenario 6 - Charlie sends 50 to Alice:
✓ SUCCESS: Charlie: 850.00, Alice: 850.00

Final State:
Alice: 850.00
Bob: 550.00
Charlie: 850.00
Total: 2250.00 (preserved - no money created/destroyed)

KEY FEATURES DEMONSTRATED:
- Balance preservation (total system balance remains constant)
- Comprehensive input validation
- Atomic transactions (both accounts updated together)
- Clear success/failure feedback
- State tracking before and after each operation
- Prevention of common transaction errors

The program showcases how modern blockchain platforms like Ethereum handle 
account-based transactions with direct balance modifications rather than 
UTXO consumption and creation.
