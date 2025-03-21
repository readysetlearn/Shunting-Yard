/* This file contains test cases for the Shunting Yard implementation.*/
#include <iostream> 
#include "ShuntingYard.cpp"

std::string queueToString(std::queue<Token> q)
{
    std::string result;
    while(!q.empty())
    {
        result += q.front().getValue();
        q.pop();
        if (!q.empty()) {
            result += " ";
        }
    }
    return result;
}


/* Test the function std::queue<Token> shuntingYard(const std::string& expr) */
void testShuntingYard()
{
	std::cout << "Now testing ShuntingYard function" << std::endl;
	
    struct TestCase {
        std::string input;
        std::string expectedOutput;
    };

    std::vector<TestCase> testCases = {
		// Expected output must not have any leading or trailing spaces, and each operand and operator must be separated by a single space.
		{"3 + 4", "3 4 +"},
		{"3 + 4 * 2 / ( 1 - 5 )", "3 4 2 * 1 5 - / +"},
        {"( 1 + 2 ) * 3", "1 2 + 3 *"},
        {"5 + ( 6 - 2 ) * 3", "5 6 2 - 3 * +"},
        {"( 7 + 3 ) / ( 2 - 1 )", "7 3 + 2 1 - /"},
        {"8 * 9 + 2", "8 9 * 2 +"},
		{"3+4*2-5!","3 4 2 * + 5 ! -"},
		{"2^3^4","2 3 4 ^ ^"},
		{"3+4*2/(1-5)^2^3","3 4 2 * 1 5 - 2 3 ^ ^ / +"},
		{"50000- 1000","50000 1000 -"},
		{"100-80+25","100 80 - 25 +"},
		{"5!", "5 !"},
		{"5! * 2", "5 ! 2 *"},
		{"3","3"},
		{"(3)","3"},
		{"(10 + 3)","10 3 +"},
		{"10.5-9.8","10.5 9.8 -"},
		{"3(5)", "3 5 *"},
		{"(4)5", "4 5 *"},
		{"(12)(15)", "12 15 *"},
		{"(1.2) (1.5)", "1.2 1.5 *"},
		{"3!*(4)", "3 ! 4 *"},
		{"3!(4)", "3 ! 4 *"},
		{"(4)3!", "4 3 ! *"},
		{"-5", "- 5"},
		{"-5 + 3", "- 5 3 +"},
		{"-5 * 3", "- 5 3 *"},
		{"(-5)", "- 5"},
		{"-(5)", "- 5"},
		{"(-5 + 3)", "- 5 3 +"},
		{"-5(3)", "- 5 3 *"},
		{"-(5 + 3)", "- 5 3 +"},
		{"-5 - 3", "- 5 3 -"},
		{"-3 + 4 * 2", "- 3 4 2 * +"},
		{"-(2+3)", "- 2 3 +"},
		{"-5!", "- 5 !"},
		{"-(5!)", "- 5 !"},
		{"(2+3)4", "2 3 + 4 *"},
		{"(2+3)(4-1)", "2 3 + 4 1 - *"},
		{"10(2+3)", "10 2 3 + *"},
		{"(2+3)!4", "2 3 + ! 4 *"},
		{"(3 + 4) * 2", "3 4 + 2 *"},
		{"",""}
    };

    for (const auto& testCase : testCases) {
        std::string input = testCase.input;
        std::queue<Token> output = shuntingYard(input);
		std::cout << std::endl;
        std::string outputStr = queueToString(output);

        if (outputStr == testCase.expectedOutput) {
            std::cout << "Test passed for input: '" << input << "'" << std::endl;
        } else {
            std::cout << "Test failed for input: '" << input << "'" << std::endl;
            std::cout << "Expected: '" << testCase.expectedOutput << "'" << std::endl;
            std::cout << "Got: '" << outputStr << "'" << std::endl;
        }
    }
}

/* Test the function long double evaluatePostfix(std::queue<Token> postfix) */
void testEvaluator()
{
	std::cout << "Now testing evaluatePostfix function" << std::endl;
	
	struct TestCase {
		std::string expression; // The mathematical expression as a string
		long double expectedResult; // The expected result of the expression
	};

	// Initialize a vector of TestCase
	std::vector<TestCase> testCases = {
		{"2 + 3", 5},
		{"6 - 5", 1},
		{"10 - 8", 2},
		{"555 - 123", 432},
		{"3 * 4", 12},
		{"3 * 2 - 1", 5},
		{"1 + 2 + 2", 5},
		{"5 * 2 - 1", 9},
		{"3 * 4 - 1", 11},
		{"1 + 2 + 3", 6},
		{"2 + 3", 5},
		{"5 - 2", 3},
		{"4 * 3", 12},
		{"15 / 3", 5},
		{"3!", 6},
		{"3! - 5", 1},
		{"(2 + 3) * 5", 25},
		{"5 + (1 + 2) * 4 - 3", 14},
		{"2.5 + 3.5", 6.0},
		{"(3 + 4) * 2 / 7", 2},
		{"(2 + 3) * (4 + 5)", 45},
		{"(2 + 3) + (4 + 5) + 6", 20},
		{"-2 + 3", 1},
		{"5 - 2 + 3", 6},
		{"5 - (2 + 3)", 0},
		{"10 + 2 * (3 - 1)", 14},
		{"(2 + 3) * 2 - 1", 9},
		{"-2 + 3 * 2.5", 5.5},
		{"2! + 3 ^ 2 - 1", 10},
		{"-(10 / 2)-2", -7},
		{"-(10 / 2) - 2 + 1.5", -5.5},
		{"(-2+10 / 2) - 2 + 1.5", 2.5}
	};

	
    // Loop through each test case and check the result
    for (const auto& testCase : testCases) {
		std::cout << std::endl;
        long double result = evaluatePostfix(shuntingYard(testCase.expression));
        if (result != testCase.expectedResult) {
            std::cout << "Failure: Expression \"" << testCase.expression 
                      << "\" evaluated to " << result 
                      << ", but expected " << testCase.expectedResult << std::endl;
        } else {
            std::cout << "Success: Expression \"" << testCase.expression 
                      << "\" evaluated correctly to " << result << std::endl;
        }
    }
}

/* Interactive mode to evaluate multiple expressions */
void interactiveMode() {
    std::string input;
    std::cout << "Enter expressions to evaluate or type 'e' to exit:" << std::endl;
    while(true)
	{
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "e" || input == "exit") break;
        try
		{
			const auto postfix = shuntingYard(input);
            const auto result = evaluatePostfix(postfix);
            std::cout << "Result: " << result << std::endl;
        } 
		catch(const std::exception& e)
		{
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
	if (argc > 1 && (std::string(argv[1]) == "--interactive" || std::string(argv[1]) == "-i")) 
	{// Enter interactive mode
        interactiveMode();
    }
	else
	{// Run tests
		std::cout << "Tests started: " << std::endl;
		testShuntingYard();
		testEvaluator();
		std::cout << "Done" << std::endl;
	}
		
	return 0;
}