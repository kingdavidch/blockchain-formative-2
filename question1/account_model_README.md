Account/Balance Model Simulation
=============================

This program simulates a basic Account/Balance model similar to how Ethereum handles transactions.

How it works:
------------
1. The program maintains a list of accounts, each containing:
   - Account name (string)
   - Current balance (float)

2. The simulation includes three users with initial balances:
   - King: 1000.0 units
   - Jack: 500.0 units
   - Queen: 750.0 units

3. The program demonstrates transactions between these users:
   - King sends 200.0 units to Jack
   - Jack sends 100.0 units to Queen
   - Queen sends 150.0 units to King

4. For each transaction, the program:
   - Validates that the sender has sufficient funds
   - Deducts the amount from the sender's balance
   - Adds the amount to the receiver's balance
   - Displays updated balances for all accounts

Code Summary:
------------
1. Account Structure:
   - Defines an Account struct with name and balance
   - Maintains an array of accounts with a maximum limit

2. Key Functions:
   - create_account(): Creates a new account with initial balance
   - find_account(): Locates an account by name
   - transfer_funds(): Handles fund transfers between accounts
   - display_balances(): Shows current balances of all accounts

3. Transaction Validation:
   - Checks if accounts exist
   - Verifies sufficient funds
   - Prevents negative balances

Expected Behavior:
----------------
When you run the program, you'll see:
1. Initial account creation and balances
2. Transaction details for each transfer
3. Updated balances after each transaction
4. Error messages for invalid transactions

To compile and run:
-----------------
gcc account_model_simulation.c -o account_model
./account_model

Note: This is a simplified implementation for educational purposes. In a real blockchain, you would need additional security measures and proper cryptographic signatures for transactions. 