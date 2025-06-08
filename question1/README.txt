UTXO Simulation Program
=====================

This program simulates a simplified UTXO (Unspent Transaction Output) model similar to how Bitcoin handles transactions.

How it works:
------------
1. The program maintains a pool of UTXOs, each containing:
   - A unique transaction ID
   - Receiver's address
   - Amount
   - Spent status

2. The simulation includes two users (Alice and Bob) with initial balances:
   - Alice: 100.0 units
   - Bob: 50.0 units

3. The program demonstrates transactions between these users:
   - Alice sends 30.0 units to Bob
   - Bob sends 20.0 units back to Alice

4. For each transaction, the program:
   - Selects appropriate UTXOs to cover the transaction amount
   - Creates new UTXOs for the receiver and any change
   - Marks spent UTXOs as used
   - Prints the updated state of available UTXOs

Sample Input/Output:
------------------
When you run the program, you'll see:
1. Initial UTXO state showing Alice and Bob's balances
2. Transaction details for Alice sending to Bob
3. Updated UTXO state after the first transaction
4. Transaction details for Bob sending to Alice
5. Final UTXO state showing remaining balances

To compile and run:
-----------------
gcc utxo_simulation.c -o utxo_simulation
./utxo_simulation

Note: The program uses random transaction IDs for demonstration purposes. 