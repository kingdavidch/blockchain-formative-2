Blockchain Mining Simulation
=========================

This program demonstrates a simplified blockchain implementation with proof-of-work mining functionality.

How it works:
------------
1. The program implements a basic blockchain structure with:
   - Blocks containing transactions, timestamps, and hashes
   - Proof-of-work mining mechanism
   - Difficulty adjustment
   - Blockchain verification

2. Key components:
   - blockchain.h: Contains structure definitions and function declarations
   - blockchain.c: Implements core blockchain functionality
   - mining.c: Demonstrates mining with different difficulty levels

3. Features:
   - Genesis block creation
   - Transaction addition
   - Block mining with adjustable difficulty
   - Mining time measurement
   - Blockchain verification

4. The mining demonstration:
   - Tests different difficulty levels (1-4)
   - Measures mining time for each difficulty
   - Shows the impact of difficulty on mining effort

To compile and run:
-----------------
1. Compile the program:
   gcc blockchain.c mining.c -o blockchain_mining

2. Run the program:
   ./blockchain_mining

Sample Output:
------------
The program will demonstrate mining with different difficulty levels and show:
- Time taken to mine each block
- The resulting block hash
- The nonce value found
- Impact of difficulty on mining time

Note: This is a simplified implementation for educational purposes. The hash function is not cryptographically secure, and the difficulty adjustment is basic. In a real blockchain, you would use proper cryptographic libraries and more sophisticated difficulty adjustment algorithms.
