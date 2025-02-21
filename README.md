# Shunting-Yard

## Overview
Implementation of the [Shunting Yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) in C++. Converts infinx expressions to [postfix expressions](https://en.wikipedia.org/wiki/Reverse_Polish_notation). 

## Features

- Supports the following binary operators: `+`, `-`, `*`, `/`, `^`
- Supports the following unary operators: `-`, `!`
- Supports parentheses: `(`, `)`
- Supports multiplication between parentheses (e.g. `(3)(5)`, `3(5)` and `(3)5`)
- Supports numbers with decimals (e.g., `2.3`)
- Main function to call: `std::queue<Token> shuntingYard(std::string& expr)` located in `ShuntingYard.cpp`

## Testing

- A suite of test cases is provided to ensure the implementation is correct. Below are the steps for running the tests:

### Running Tests

```
bash
g++ -o Test_Shunting_Yard Test_Shunting_Yard.cpp
./Test_Shunting_Yard
```


## Known Limitations
- Input expressions are not validated for correctness. For example, `^5` has ouptut `5^` without any errors.
