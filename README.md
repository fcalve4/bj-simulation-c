# Blackjack Simulation

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
- A terminal or command-line interface.

### Installation

Clone the repository:

```bash
git clone git@github.com:fcalve4/bj-simulation-c.git
```

Navigate to the project directory:

```bash
cd bj-simulation-c
```

Build the project:

```bash
make
```

---

## Usage

To run the simulator, provide a **strategy CSV file** as input:

```bash
./bin/bj_simulation <strategy.csv>
```

### Example

If you have a file `basic_strategy.csv`, execute:

```bash
./bin/bj_simulation basic_strategy.csv
```

This will simulate blackjack hands based on the specified strategy and provide detailed output of the simulation results.

---

## Strategy File Format

The input CSV file should contain the blackjack strategy matrix, formatted as follows:

- Rows represent the **player's hand total** (e.g., `hard15`, `soft18`).
- Columns represent the **dealer's up card** (e.g., `2`, `3`, `4`...`A`).
- Each cell contains the **optimal play** (`H` for hit, `S` for stand, `D` for double, etc.).

Example snippet:

|         | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | A   |
|---------|------|------|------|------|------|------|------|------|------|------|
| hard15  | H    | H    | H    | H    | S    | H    | H    | H    | H    | H    |
| soft18  | S    | S    | S    | S    | S    | H    | H    | H    | H    | H    |

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
