# Shunting-Yard

## Overview
C++ implementation of the Shunting Yard algorithm for parsing mathematical expressions.

## Features
- Supports the following operators: `+`, `-`, `*`, `/`, `^`, `!`, and parentheses `(`, `)`
- Handles numbers with decimals (e.g., `2.3`)
- Handles multiplication between parentheses (e.g. `(3)(5)`, `3(5)` and `(3)5`)
- Main function to call: `std::queue<Token> shuntingYard(std::string& expr)` located in `ShuntingYard.cpp`

## Usage
To compile and run the code, use the following commands:

```bash
g++ -o Test_Shunting_Yard Test_Shunting_Yard.cpp
./Test_Shunting_Yard