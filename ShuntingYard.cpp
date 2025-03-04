/*Shunting Yard algorithm without evaluation
Algorithm from https://en.wikipedia.org/wiki/Shunting_yard_algorithm#The_algorithm_in_detail*/
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <map>
#include <cassert>
#include <stdexcept>


class Token
{
	public:
		enum Type { NUMBER, OPERATOR, PARENTHESIS };
		enum Associativity { LEFT, RIGHT };

		Token(Type type, const std::string& value) : type(type), value(value)
		{
			if (value.empty()) // Check for empty string
			{
				throw std::invalid_argument("Token created with an empty string. Type: " + std::to_string(type));
			}
		}

		Token(Type type, const char value) : type(type), value(std::string(1, value)), precedence(setPrecedence(value)), associativity(setAssociativity(value)), unary((value == '!') ? true : false)
		{
			if (value == '\0') // Check for null character
			{
				throw std::invalid_argument("Token created with a null character. Type: " + std::to_string(type));
			}
		}
		
		// Constructor for unary negation operator (e.g. the '-' in '-5')
		Token() : type(OPERATOR), value("-"), precedence(4), associativity(RIGHT), unary(true)
		{
			// Note: this constructor does not do any validation
		}

		
		// const member functions
		Type getType() const { return type; }
		std::string getValue() const { return value; }
		short getPrecedence() const { return precedence; }
		Associativity getAssociativity() const { return associativity; }
		bool isUnary() const { return unary; }
		
	private:
		Type type;
		std::string value;
		short precedence;
		Associativity associativity;
		bool unary;
		
		// Function to determine the precedence of an operator. The higher the number, the higher the precedence, the sooner they get evaluated.
		short setPrecedence (char op) const
		{
			switch (op)
			{
				case '+':
				case '-': // This is binary subtraction, not unary negation
					return 1;
				case '*':
				case '/':
					return 2;
				case '^':
					return 3;
				case '!':
					return 5; // Precedence level 4 is used for unary negation
				case '(':
				case ')':
					return 6;
				default:
					throw std::invalid_argument("Error: Operator '" + std::string(1, op) + "' is not recognized.");
			}
		}

		// Function to determine the associativity of an operator
		Associativity setAssociativity(char op) const
		{
			switch (op) {
				case '+':
				case '-':
				case '*':
				case '/':
				case '(':
				case ')':
					return LEFT;
				case '^':
				case '!':
					return RIGHT; // Right associative for '^' and unary '!'
				default:
					throw std::invalid_argument("Error: Operator '" + std::string(1, op) + "' is not recognized.");
			}
		}
};

/* Return true if an implicit multiplication exists between the previous token and the current one
	Examples: (2)(3), 2(3), (2)3 */
bool isImpliedMultiplication(const Token& pre, const Token& cur)
{
    if(pre.getValue() == ")" && cur.getValue() == "(") return true; // e.g. (2)(3)
    if(pre.getType() == Token::NUMBER && cur.getValue() == "(") return true; // e.g. 2(3)
    if(pre.getValue() == ")" && cur.getType() == Token::NUMBER) return true; // e.g. (2)3
    if(pre.getValue() == "!" && cur.getValue() == "(") return true; // e.g. 3!(4)
    if(pre.getValue() == ")" && cur.getValue() == "!") return true; // e.g. (4)3!
	return false;
}

unsigned long long factorial(const unsigned long long n)
{
	if(n == 0)
	{
		return 1;
	}
	else
	{
		unsigned long long f = 1;
		for(unsigned long long i = 1; i <= n; i++)
		{
			f *= i;
		}
		return f;
	}
}


long double performOperation(const Token op, const long double numLeft, const long double numRight = 0.0)
{
    if(op.getType() != Token::OPERATOR)
    {
        throw std::invalid_argument("Token is not an operator: " + op.getValue());
    }
    
    if(op.isUnary()) // Unary operator
    {
        switch(op.getValue()[0]) // Convert return value of getValue() to char for use in switch
        {
            case '-':
                return -numLeft;
            case '!':
                if(numLeft < 0)
                    throw std::invalid_argument("Factorial is not defined for negative numbers.");
                else
                    return factorial(numLeft);
            default:
                throw std::invalid_argument("Invalid unary operator: " + std::string(1, op.getValue()[0]));        
        }
    }
    else // Binary operator
    {
        switch(op.getValue()[0])
        {
            case '+':
                return numLeft + numRight;
            case '-':
                return numLeft - numRight;
            case '*':
                return numLeft * numRight;
            case '/':
                if (numRight == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return numLeft / numRight;
            case '^':
                return pow(numLeft, numRight);
            default:
                throw std::invalid_argument("Invalid binary operator: " + std::string(1, op.getValue()[0]));        
        }
    }
}




/*converts an infinx expression to a postfix expression*/
std::queue<Token> shuntingYard(std::string& expr)
{
	std::queue<Token> output;
	std::stack<Token> operators;
	std::string number;
	Token previous = Token(Token::NUMBER, "-1"); // Arbitrary value for starting
	bool firstIteration = true; // Used by isImpliedMultiplication()
	const auto isNum = previous; // Arbitrary token with type number
	
	for(auto c : expr)
	{
		if (std::isspace(static_cast<unsigned char>(c))) {
            continue; // Skip this iteration if c is whitespace
        }
		if(std::isdigit(c) || c == '.')
		{
			if(!operators.empty() && operators.top().isUnary() && operators.top().getValue() == "-")
			{
				output.push(operators.top());
				operators.pop();
			}
			number += c;
		}
		else if(c == '!') // Factorial has highest precedence and doesn't go on the operator stack
		{
			if(previous.getValue() == ")")
			{
				operators.push(Token(Token::OPERATOR, '*')); // Implicit multiplication e.g. (2)3!
			}
			if(!number.empty())
			{
				output.push(Token(Token::NUMBER, number));
				number.clear();
			}

			output.push(Token(Token::OPERATOR, '!'));
			previous = Token(Token::OPERATOR, '!');
		}
		else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
		{
			if(!number.empty())
			{
				output.push(Token(Token::NUMBER, number));
				number.clear();
				previous = isNum;
			}
			// Determine whether token is unary negation or binary operator
			Token t = (c == '-' && (firstIteration || previous.getType() != Token::NUMBER)) ? Token() : Token(Token::OPERATOR, c);
			while(!operators.empty() && operators.top().getValue() != "(" && (operators.top().getPrecedence() > t.getPrecedence() || (operators.top().getPrecedence() == t.getPrecedence() && t.getAssociativity() == Token::LEFT)))
			{
				output.push(operators.top());
				operators.pop();
			}
			operators.push(t);
			previous = t;
		}
		else if(c == '(')
		{
			if(!number.empty())
			{
				output.push(Token(Token::NUMBER, number));
				number.clear();
				previous = isNum;
			}
			if(!operators.empty() && operators.top().isUnary() && operators.top().getValue() == "-")
			{
				output.push(operators.top());
				operators.pop();
			}
			if(!firstIteration && isImpliedMultiplication(previous, Token(Token::PARENTHESIS, '(')))
			{
				operators.push(Token(Token::OPERATOR, '*'));
			}
			operators.push(Token(Token::PARENTHESIS, '('));
			previous = Token(Token::PARENTHESIS, '(');
		}
		else if(c == ')')
		{
			if(!number.empty())
			{
				output.push(Token(Token::NUMBER, number));
				number.clear();
				previous = isNum;
			}
			if(!firstIteration && isImpliedMultiplication(previous, Token(Token::PARENTHESIS, ')')))
			{
				operators.push(Token(Token::OPERATOR, '*'));
			}
			while(operators.top().getValue() != "(")
			{
				output.push(Token(Token::OPERATOR, operators.top().getValue()));
				operators.pop();
				assert(!operators.empty() && "Mismatched parentheses");
			}
			assert(operators.top().getValue() == "(" && "Expected '('");
			operators.pop(); // Pop the '('
			previous = Token(Token::PARENTHESIS, ')');
		}
		else
		{
			// uh oh
			throw std::runtime_error("Invalid token: " + std::string(1, c));
		}
		firstIteration = false;
	}
	
	// Handle last token being a number
	if(!number.empty())
	{
		output.push(Token(Token::NUMBER, number));
		number.clear();
		if(!firstIteration && isImpliedMultiplication(previous, isNum))
		{
			output.push(Token(Token::OPERATOR, '*'));
		}
	}
	
	while(!operators.empty())
	{
		output.push(Token(Token::OPERATOR, operators.top().getValue()));
		operators.pop();
	}
	
	return output;
}

/* Evaluates a math expression.
This function is very similar to shuntingYard(). */
long double expression_evaluator(const std::string& expr)
{
	std::stack<Token> operands;
	std::stack<Token> operators;
	std::string number;
	Token previous = Token(Token::NUMBER, "-1"); // Arbitrary value for starting
	bool firstIteration = true; // Used by isImpliedMultiplication()
	const auto isNum = previous; // Arbitrary token with type number
	
	for(auto c : expr)
	{
		if (std::isspace(static_cast<unsigned char>(c))) {
            continue; // Skip this iteration if c is whitespace
        }
		if(std::isdigit(c) || c == '.')
		{
			if(!operators.empty() && operators.top().isUnary() && operators.top().getValue() == "-")
			{
				operands.push(operators.top());
				operators.pop();
			}
			number += c;
		}
		else if(c == '!') // Factorial has highest precedence and doesn't go on the operator stack
		{
			if(previous.getValue() == ")")
			{
				operators.push(Token(Token::OPERATOR, '*')); // Implicit multiplication e.g. (2)3!
			}
			if(!number.empty())
			{
				operands.push(Token(Token::NUMBER, number));
				number.clear();
			}

			operands.push(Token(Token::OPERATOR, '!'));
			previous = Token(Token::OPERATOR, '!');
		}
		else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
		{
			if(!number.empty())
			{
				operands.push(Token(Token::NUMBER, number));
				number.clear();
				previous = isNum;
			}
			// Determine whether token is unary negation or binary operator
			Token t = (c == '-' && (firstIteration || previous.getType() != Token::NUMBER)) ? Token() : Token(Token::OPERATOR, c);
			if (t.isUnary())
			{
				operands.push(t);
			}
			else
			{
				while(!operators.empty() && operators.top().getValue() != "(" && (operators.top().getPrecedence() > t.getPrecedence() || (operators.top().getPrecedence() == t.getPrecedence() && t.getAssociativity() == Token::LEFT)))
				{
					operands.push(operators.top());
					operators.pop();
				}
				operators.push(t);
			}
			previous = t;
		}
		else if(c == ')')
		{
			if(!number.empty())
			{
				operands.push(Token(Token::NUMBER, number));
				number.clear();
				previous = isNum;
			}
			if(!firstIteration && isImpliedMultiplication(previous, Token(Token::PARENTHESIS, ')')))
			{
				operators.push(Token(Token::OPERATOR, '*'));
			}
			while(operators.top().getValue() != "(")
			{
				operands.push(operators.top());
				operators.pop();
				assert(!operators.empty() && "Mismatched parentheses");
			}
			assert(operators.top().getValue() == "(" && "Expected '('");
			operators.pop(); // Pop the '('
			previous = Token(Token::PARENTHESIS, ')');
		}

		else if(c == ')')
		{
			if(!number.empty())
			{
				operands.push(Token(Token::NUMBER, number));
				number.clear();
				previous = isNum;
			}
			if(!firstIteration && isImpliedMultiplication(previous, Token(Token::PARENTHESIS, ')')))
			{
				operators.push(Token(Token::OPERATOR, '*'));
			}
			while(operators.top().getValue() != "(")
			{
				operands.push(Token(Token::OPERATOR, operators.top().getValue()));
				operators.pop();
				assert(!operators.empty() && "Mismatched parentheses");
			}
			assert(operators.top().getValue() == "(" && "Expected '('");
			operators.pop(); // Pop the '('
			previous = Token(Token::PARENTHESIS, ')');
		}
		else
		{
			// uh oh
			throw std::runtime_error("Invalid token: " + std::string(1, c));
		}
		firstIteration = false;
	}
	
	// Handle last token being a number
	if(!number.empty())
	{
		operands.push(Token(Token::NUMBER, number));
		number.clear();
		if(!firstIteration && isImpliedMultiplication(previous, isNum))
		{
			operands.push(Token(Token::OPERATOR, '*'));
		}
	}
	
	while(!operators.empty())
	{
		const auto op = operators.top();
		operators.pop();
		const auto numRight = operands.top();
		operands.pop();
		const auto numLeft = operands.top();
		operands.pop();

		if (numRight.getType() == Token::OPERATOR) {
			const auto result = performOperation(numRight, 0.0);
			operands.push(Token(Token::NUMBER, std::to_string(result)));
			const auto newNumRight = std::stold(operands.top().getValue());
			operands.pop();
			const auto result2 = performOperation(op, std::stold(numLeft.getValue()), newNumRight);
			operands.push(Token(Token::NUMBER, std::to_string(result2)));
		} else if (numLeft.getType() == Token::OPERATOR) {
			const auto result = performOperation(numLeft, 0.0);
			operands.push(Token(Token::NUMBER, std::to_string(result)));
			const auto newNumLeft = std::stold(operands.top().getValue());
			operands.pop();
			const auto result2 = performOperation(op, newNumLeft, std::stold(numRight.getValue()));
			operands.push(Token(Token::NUMBER, std::to_string(result2)));
		} else {
			const auto result = performOperation(op, std::stold(numLeft.getValue()), std::stold(numRight.getValue()));
			operands.push(Token(Token::NUMBER, std::to_string(result)));
		}
	}




	
	return std::stold(operands.top().getValue());
}