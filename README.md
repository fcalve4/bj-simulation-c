# Blackjack Simulator

A comprehensive **Blackjack Simulator** written in C, inspired by Norm Wattenberger's **CVCX Blackjack Analyzer**. This project enables detailed analysis of various blackjack strategies, simulating hands and calculating expected values (EV) for many combinations of rules.

## Features

- Supports importing custom strategy files in CSV format.
- Simulates blackjack hands with configurable parameters.
- Calculates and displays "Return to Player" or "RTP" values for different rule configurations.
- Optimized for performance to handle large simulations efficiently, easily allowing for simulation of hundreds of millions of hands.

---

## Getting Started

### Prerequisites

To run this simulator, ensure the following are installed on your system:

- **GCC** or another C compiler.
- **Make** build tool.

### Installation

Clone the repository:

```bash
git clone git@github.com:fcalve4/blackjack-simulator.git
```

Navigate to the project directory:

```bash
cd blackjack-simulator
```

Build the project:

```bash
make
```

---

## Usage

To run the simulator, provide a **strategy CSV file** as input and a **output TXT file** for the output:

```bash
./bin/bj_sim <strategy.csv> <output.txt>
```

### Example

If you have files `basic_strategy.csv` and `output.txt`, execute:

```bash
./bin/bj_sim basic_strategy.csv output.txt
```

### Optional Flags

The following flags can be used to modify the simulation behavior:

| Flag    | Description | Default Value |
| -------- | ------- | ------- |
| --num-decks |	Number of decks used in the game | 6 |
| -h17 |	Dealer hits on soft 17 (1: enabled, 0: disabled) | 1 |
| -ls	| Late surrender allowed (1: enabled, 0: disabled) | 0 |
| -das |	Double after split allowed (1: enabled, 0: disabled) | 1 |
| -rsa | Re-splitting aces allowed (1: enabled, 0: disabled)	| 1 |
| -enhc | European No Hole Card rule (1: enabled, 0: disabled) |	0 |
| --max-splits | Maximum number of splits allowed |	4 |
| --bj-pay | Blackjack payout multiplier (e.g., 1.5 for 3:2) |	1.5 |
| -pen Deck | penetration percentage (e.g., 0.75 for 75%)	| 0.75 |
| --num-simulations | Number of hands to simulate	| 1,000,000 |
| -bankroll | Starting bankroll for the simulation |	10,000 |
| -wager | Wager amount per hand | 10 |

### Optional Flags

To run a simulation with a specific set of rules, use the flags like so:

```bash
./bin/bj_sim basic_strategy.csv output.txt --num-decks 8 -ls 1 --bj-pay 1.2 --num-simulations 500000
```

This example sets:
- 8 decks in play.
- Late surrender enabled.
- Blackjack payout set to 6:5.
- 5,000,000 hands simulated.

---
## Strategy File Format

The input CSV file contains the blackjack strategy matrix, formatted with rows representing the **player's hand totals** and columns corresponding to the **dealer's upcard**. Each cell specifies the optimal play based on the key provided.

### Example Format

```csv
H,H,H,H,H,H,H,H,H,H,hard4
H,H,H,H,H,H,H,H,H,H,hard5
H,H,H,H,H,H,H,H,H,H,hard6
...
S,S,S,S,S,S,S,S,S,S,hard20
S,S,S,S,S,S,S,S,S,S,hard21
P,P,P,P,P,S,P,P,S,S,split9
S,S,S,S,S,S,S,S,S,S,split10
P,P,P,P,P,P,P,P,P,P,splitA

2,3,4,5,6,7,8,9,10,11,Dealer Upcards
```

### Key
- **H**: Hit  
- **S**: Stand  
- **P**: Split
- **D**: Double/Hit  
- **T**: Double/Stand  
- **X**: Surrender/Stand  
- **Y**: Surrender/Hit  
- **Z**: Surrender/Split  

### Notes
- The **order of the rows** is critical and hardcoded into the simulation logic.
- The last column of each row identifies the player's hand type (e.g., `hard4`, `soft18`, `splitA`).
- The last row is implied and lists the dealer's upcards (`2` through `11`, where `11` represents the Ace) for reference.

This detailed format ensures accurate parsing and correct strategy application during the simulation.

---

## Contributing

Contributions are welcome! To contribute:

1. Fork this repository.
2. Create a new branch for your feature/bugfix.
3. Submit a pull request with a detailed explanation of your changes.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Acknowledgments

- Inspired by **Norm Wattenberger's CVCX Blackjack Analyzer**.
- Thanks to all contributors and advantage blackjack enthusiasts for their support.
