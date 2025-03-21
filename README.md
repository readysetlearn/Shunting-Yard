# Shunting-Yard

## Overview
An implementation of the [Shunting Yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) as well as a function to evaluate postfix expressions. The Shunting Yard algorithm is used to parse mathematical expressions specified in infix notation and convert them into postfix notation (aka [postfix expressions](https://en.wikipedia.org/wiki/Reverse_Polish_notation)). The postfix evaluation function then can be used to compute the result of the postfix expression.

## Key Components

1. **Shunting Yard Algorithm**:
   - **Purpose**: Converts infix expressions to postfix expressions.
   - **How it works**: The algorithm uses a stack to rearrange the operators and operands in the expression according to their precedence and associativity.
   - **Example**:
     ```plaintext
     Input: 10 - 2 * 3
     Output: 10 2 3 * -
     ```

2. **Postfix Evaluation Function**:
   - **Purpose**: Evaluates postfix expressions to compute the final result.
   - **How it works**: The function uses a stack to process the operands and operators in the postfix expression.
   - **Example**:
     ```plaintext
     Input: 10 - 2 * 3
     Output: 4
     ```

## Features

- Supports the following binary operators: `+`, `-`, `*`, `/`, `^`
- Supports the following unary operators: `-`, `!`
- Supports parentheses: `(`, `)`
- Supports multiplication between parentheses (e.g. `(3)(5)`, `3(5)` and `(3)5`)
- Supports numbers with decimals (e.g., `2.3`)
- Main functions to call: `std::queue<Token> shuntingYard(std::string& expr)` and `long double evaluatePostfix(std::queue<Token> postfix)` located in `ShuntingYard.cpp`

## Usage

To use these functions, simply include the respective header files and call the functions with the appropriate expressions.

### Example Usage

```cpp
#include "ShuntingYard.cpp"
#include <iostream>
#include <queue>

int main() {
    std::string infix_expression = "10 - 2 * 3";
    std::queue<Token> postfix_expression = shuntingYard(infix_expression);
    long double result = evaluatePostfix(postfix_expression);
    std::cout << "Result: " << result << std::endl;
    // Output: Result: 4
    return 0;
}
```

## Testing

- A suite of test cases are provided in `Test_Shunting_Yard.cpp` to ensure the implementation is correct. Below are the steps to run the tests:

### Running Tests

```bash
g++ -o Test_Shunting_Yard Test_Shunting_Yard.cpp
./Test_Shunting_Yard
```


## Known Limitations
- The `shuntingYard` function does not validate input for correctness. For example, `^5` has output `5^` and `3 ** 4` has the output `3 * 4 *` without any errors.
