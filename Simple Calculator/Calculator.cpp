#include "Calculator.h"
#include <iostream>
#include <cctype>

void Calculator::run()
{
	std::cout << "Welcome to the Calculator! Input first number: ";
	while (true)
	{
		//Check if input is valid
		double first = readValidNumber();

		//Input was valid, ask for operator...
		char op = readValidOperator();
		
		//Ask for second input
		double second = readValidNumber();

		double r = result(first, op, second);
		std::cout << "Result: " << r << '\n';
		break;
	}
}

void Calculator::removeSpaces(std::string& str)
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

double Calculator::readValidNumber()
{
	std::string line;
	while (true)
	{
		std::cout << "Insert number: ";
		if (!std::getline(std::cin, line))
		{
			//Will practically never execute. Only when user presses Ctrl + Z or something.
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		try
		{
			size_t pos;
			double input = std::stod(line, &pos);
			if (pos == line.size())
				return input;
			std::cout << "Please enter a valid number (no letters, characters or operators).\n";
		}
		catch (...)
		{
			std::cout << "Invalid input, expression may not include alphabets, characters or operators.\n";
		}
	}
}

char Calculator::readValidOperator()
{
	std::string line;
	while (true)
	{
		std::cout << "Insert operator: ";
		if (!std::getline(std::cin, line))
		{
			//Will practically never execute. Only when user presses Ctrl + Z or something.
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		try
		{
			if (line.length() == 1 && (line[0] == '+' || line[0] == '-' || line[0] == '*' || line[0] == '/'))
			{
				char c = line[0];
				return c;
			}
			std::cout << "Please enter a valid operator (+, -, *, or /).\n";
		}
		catch (...)
		{
			std::cout << "Invalid input, Please enter a valid operator (+, -, *, or /).\n";
		}
	}
}

double Calculator::result(double x, char o, double y)
{
	switch (o)
	{
	case '+':
		return add(x, y);
	case '-':
		return substract(x, y);
	case '*':
		return multiply(x, y);
	case '/':
		return divide(x,y);
	default:
		std::cout << "Something went horribly wrong!\n";
		break;
	}
}