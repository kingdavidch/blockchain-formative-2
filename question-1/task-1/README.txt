# UTXO Simulation Program

## Overview
This C program simulates a simplified UTXO (Unspent Transaction Output) model similar to Bitcoin's transaction system. It demonstrates how transactions consume existing UTXOs and create new ones.

## How It Works
1. **UTXOs** are stored in a global pool with transaction ID, owner address, amount, and spent status
2. **Transactions** select UTXOs from sender's address to cover the transfer amount
3. Selected UTXOs are marked as "spent" and cannot be reused
4. New UTXOs are created: one for receiver, one for sender's change (if any)
5. The system tracks all UTXOs and can display available (unspent) ones

## Key Features
- Struct-based UTXO and Transaction definitions
- UTXO selection algorithm (greedy approach)
- Balance calculation from unspent UTXOs
- Automatic change handling
- Transaction validation (sufficient funds check)
- Complete transaction history tracking

## Compilation
```bash
gcc -o utxo_simulation utxo_simulation.c
```

## Execution
```bash
./utxo_simulation
```

## Sample Input/Output
The program runs automatically with predefined scenarios:

### Initial State
- **Alice**: 150.0 (from GENESIS1: 100.0, GENESIS2: 50.0)
- **Bob**: 100.0 (from GENESIS3: 75.0, GENESIS4: 25.0)

### Transaction 1: Alice sends 30 to Bob
- Alice's UTXOs (100.0, 50.0) are spent
- New UTXOs created: Bob gets 30.0, Alice gets 120.0 change
- **Result**: Alice: 120.0, Bob: 130.0

### Transaction 2: Bob sends 40 to Charlie
- Bob's available UTXOs are spent to cover 40.0
- New UTXOs: Charlie gets 40.0, Bob gets remaining change
- **Result**: Bob balance decreases, Charlie: 40.0

### Transaction 3: Alice tries to send 200 (fails)
- Validation catches insufficient funds
- No UTXOs are modified

### Transaction 4: Charlie sends 15 to Alice
- Charlie's UTXO is partially spent
- Alice receives 15.0, Charlie gets 25.0 change

## Final Output Example
```
=== ACCOUNT BALANCES ===
Alice: 135.00
Bob: 90.00
Charlie: 25.00

=== AVAILABLE UTXOs ===
TX_ID		Address		Amount
----------------------------------------
TX100		Alice		120.00
TX102		Bob		90.00
TX103		Alice		15.00
TX104		Charlie		25.00
```

## Key Concepts Demonstrated
The program demonstrates UTXO consumption, creation, and the immutable nature of blockchain transactions where spent outputs cannot be reused.
