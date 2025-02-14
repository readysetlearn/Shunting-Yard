/*Shunting Yard algorithm without evaluation
Algorithm from https://en.wikipedia.org/wiki/Shunting_yard_algorithm#The_algorithm_in_detail*/
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <cctype>
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

		Token(Type type, const char value) : type(type), value(std::string(1, value)), precedence(setPrecedence(value)), associativity(setAssociativity(value))
		{
			if (value == '\0') // Check for null character
			{
				throw std::invalid_argument("Token created with a null character. Type: " + std::to_string(type));
			}
		}
		
		// const member functions
		Type getType() const { return type; }
		std::string getValue() const { return value; }
		short getPrecedence() const { return precedence; }
		Associativity getAssociativity() const { return associativity; }
		
	private:
		const Type type;
		const std::string value;
		short precedence;
		Associativity associativity;
		bool unary;
		
		// Function to determine the precedence of an operator. Higher the number the higher the precedence.
		short setPrecedence (char op) const
		{
			switch (op)
			{
				case '+':
				case '-':
					return 1;
				case '*':
				case '/':
					return 2;
				case '^':
					return 3;
				case '!':
					return 4;
				case '(':
				case ')':
					return 5;
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


/*converts an infinx expression to a postfix expression*/
std::queue<Token> shuntingYard(std::string& expr)
{
	std::queue<Token> output;
	std::stack<Token> operators;
	std::string number;
	
	for(auto c : expr)
	{
		if (std::isspace(static_cast<unsigned char>(c))) {
            continue; // Skip this iteration if c is whitespace
        }
		else if(std::isdigit(c) || c == '.')
		{
			number += c;
		}
		else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '!')
		{
			if(!number.empty())
			{
				output.push(Token(Token::NUMBER, number));
				number.clear();
			}
			Token t(Token::OPERATOR, c);
			while(!operators.empty() && operators.top().getValue() != "(" && (operators.top().getPrecedence() > t.getPrecedence() || (operators.top().getPrecedence() == t.getPrecedence() && t.getAssociativity() == Token::LEFT)))
			{
				output.push(Token(Token::OPERATOR, operators.top().getValue()));
				operators.pop();
			}
			operators.push(t);
		}
		else if(c == '(')
		{
			operators.push(Token(Token::PARENTHESIS, '('));
		}
		else if(c == ')')
		{
			if(!number.empty())
			{
				output.push(Token(Token::NUMBER, number));
				number.clear();
			}
			while(operators.top().getValue() != "(")
			{
				output.push(Token(Token::OPERATOR, operators.top().getValue()));
				operators.pop();
				assert(!operators.empty() && "Mismatched parentheses");
			}
			assert(operators.top().getValue() == "(" && "Expected '('");
			operators.pop(); // Pop the '('
		}
		else
		{
			// uh oh
			throw std::runtime_error("Invalid token: " + c);
		}
	}
	
	// Handle last token being a number
	if(!number.empty())
	{
		output.push(Token(Token::NUMBER, number));
		number.clear();
	}
	
	while(!operators.empty())
	{
		output.push(Token(Token::OPERATOR, operators.top().getValue()));
		operators.pop();
	}
	
	return output;
}
