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

void testShuntingYard()
{
    struct TestCase {
        std::string input;
        std::string expectedOutput;
    };

    std::vector<TestCase> testCases = {
		// Expected output must not have any leading or trailing spaces, and each operand and operator must be separated by a single space.
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
		{"3","3"},
		{"(3)","3"},
		{"(10 + 3)","10 3 +"},
		{"10.5-9.8","10.5 9.8 -"},
		{"3(5)", "3 5 *"},
		{"(4)5", "4 5 *"},
		{"(12)(15)", "12 15 *"},
		{"(1.2) (1.5)", "1.2 1.5 *"},
		{"",""},
		{"-5", "- 5"},
		{"-5 + 3", "- 5 3 +"},
		{"-5 * 3", "- 5 3 *"},
		{"(-5)", "- 5"},
		{"(-5 + 3)", "- 5 3 +"},
		{"-5(3)", "- 5 3 *"},
		{"-(5 + 3)", "- 5 3 +"},
		{"-5 - 3", "- 5 3 -"}
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

int main()
{
	std::cout << "Tests started: " << std::endl;
	testShuntingYard();
	return 0;
}