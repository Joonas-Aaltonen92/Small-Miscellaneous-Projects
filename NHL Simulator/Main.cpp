#include <iostream>
#include "League.h"

int main()
{
	League nhl;
	nhl.SimulateSeason();
	std::cin.get();
	nhl.printTopScorersSeason();
	std::cin.get();
	nhl.simulatePlayoffs();
	nhl.printFinalStandings();
	std::cin.get();
	nhl.printTopScorersPlayoffs();
	std::cin.get();
	nhl.endSeason();
	std::cin.get();
	nhl.SimulateSeason();
	std::cin.get();
	nhl.printTopScorersSeason();
	std::cin.get();
	nhl.simulatePlayoffs();
	nhl.printFinalStandings();
	std::cin.get();
	nhl.printTopScorersPlayoffs();
	std::cin.get();
	nhl.endSeason();
	std::cin.get();

	//Finished!
	//Could add saving/loading, and better user input, but meh.

	return 0;
}