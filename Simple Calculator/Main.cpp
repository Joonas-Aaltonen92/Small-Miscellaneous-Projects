#include <iostream>
#include "Calculator.h"

int main()
{
	Calculator calculator;
	calculator.run();
	std::cout << "Press Enter to exit...";
	std::cin.get();
	return 0;
}