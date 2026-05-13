#pragma once
#include <string>

class Calculator
{
	public:
		Calculator() {}

		void run();

		void removeSpaces(std::string& str);
		double readValidNumber();
		char readValidOperator();

		double add(double x, double y) { return x + y; }
		double substract(double x, double y) { return x - y; }
		double multiply(double x, double y) { return x * y; }
        double divide(double x, double y)
        {
			if (y == 0)
			{
				printf("Cannot divide by zero!");
				return -1;
			}
			else
				return x / y;
        }
		double result(double x, char o, double y);
};

