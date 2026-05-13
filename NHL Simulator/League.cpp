#include "League.h"

League::League() {
	teams.reserve(32);

	teams.emplace_back(std::make_shared<Team>("Buffalo Sabres", "BUF", "ATLANTIC", "EASTERN"));					//0
	teams.emplace_back(std::make_shared<Team>("Boston Bruins", "BOS", "ATLANTIC", "EASTERN"));					//1
	teams.emplace_back(std::make_shared<Team>("Detroit Red Wings", "DET", "ATLANTIC", "EASTERN"));				//2
	teams.emplace_back(std::make_shared<Team>("Florida Panthers", "FLA", "ATLANTIC", "EASTERN"));				//3
	teams.emplace_back(std::make_shared<Team>("Montreal Canadiens", "MTL", "ATLANTIC", "EASTERN"));				//4
	teams.emplace_back(std::make_shared<Team>("Ottawa Senators", "OTT", "ATLANTIC", "EASTERN"));				//5
	teams.emplace_back(std::make_shared<Team>("Tampa Bay Lightning", "TBL", "ATLANTIC", "EASTERN"));			//6
	teams.emplace_back(std::make_shared<Team>("Toronto Maple Leafs", "TOR", "ATLANTIC", "EASTERN"));			//7
	teams.emplace_back(std::make_shared<Team>("Carolina Hurricanes", "CAR", "METROPOLITAN", "EASTERN"));		//8
	teams.emplace_back(std::make_shared<Team>("Columbus Blue Jackets", "CBJ", "METROPOLITAN", "EASTERN"));		//9
	teams.emplace_back(std::make_shared<Team>("New Jersey Devils", "NJD", "METROPOLITAN", "EASTERN"));			//10
	teams.emplace_back(std::make_shared<Team>("New York Islanders", "NYI", "METROPOLITAN", "EASTERN"));			//11
	teams.emplace_back(std::make_shared<Team>("New York Rangers", "NYR", "METROPOLITAN", "EASTERN"));			//12
	teams.emplace_back(std::make_shared<Team>("Philadelphia Flyers", "PHI", "METROPOLITAN", "EASTERN"));		//13
	teams.emplace_back(std::make_shared<Team>("Pittsburgh Penguins", "PIT", "METROPOLITAN", "EASTERN"));		//14
	teams.emplace_back(std::make_shared<Team>("Washington Capitals", "WSH", "METROPOLITAN", "EASTERN"));		//15
	teams.emplace_back(std::make_shared<Team>("Chicago Blackhawks", "CHI", "CENTRAL", "WESTERN"));				//16
	teams.emplace_back(std::make_shared<Team>("Colorado Avalanche", "COL", "CENTRAL", "WESTERN"));				//17
	teams.emplace_back(std::make_shared<Team>("Dallas Stars", "DAL", "CENTRAL", "WESTERN"));					//18
	teams.emplace_back(std::make_shared<Team>("Minnesota Wild", "MIN", "CENTRAL", "WESTERN"));					//19
	teams.emplace_back(std::make_shared<Team>("Nashville Predators", "NSH", "CENTRAL", "WESTERN"));				//20
	teams.emplace_back(std::make_shared<Team>("St. Louis Blues", "STL", "CENTRAL", "WESTERN"));					//21
	teams.emplace_back(std::make_shared<Team>("Utah Mammoth", "UTA", "CENTRAL", "WESTERN"));					//22
	teams.emplace_back(std::make_shared<Team>("Winnipeg Jets", "WPG", "CENTRAL", "WESTERN"));					//23
	teams.emplace_back(std::make_shared<Team>("Anaheim Ducks", "ANA", "PACIFIC", "WESTERN"));					//24
	teams.emplace_back(std::make_shared<Team>("Calgary Flames", "CGY", "PACIFIC", "WESTERN"));					//25
	teams.emplace_back(std::make_shared<Team>("Edmonton Oilers", "EDM", "PACIFIC", "WESTERN"));					//26
	teams.emplace_back(std::make_shared<Team>("Los Angeles Kings", "LAK", "PACIFIC", "WESTERN"));				//27
	teams.emplace_back(std::make_shared<Team>("San Jose Sharks", "SJS", "PACIFIC", "WESTERN"));					//28
	teams.emplace_back(std::make_shared<Team>("Seattle Kraken", "SEA", "PACIFIC", "WESTERN"));					//29
	teams.emplace_back(std::make_shared<Team>("Vancouver Canucks", "VAN", "PACIFIC", "WESTERN"));				//30
	teams.emplace_back(std::make_shared<Team>("Vegas Golden Knights", "VGK", "PACIFIC", "WESTERN"));			//31

	top3.reserve(12);
	wildCards.reserve(4);
	finalStandings.reserve(32);
	collectAllPlayers();
	draftPool.reserve(32 * 7); //7 rounds, 32 players per round
}

int League::sampleShots(std::shared_ptr<Team> team, bool isHome, double baseShots, int oppDef, int oppGk) {
	std::mt19937 rng{ std::random_device{}() };
	double venueMult = isHome ? team->homeAdv : team->awayAdv;
	double lambda = baseShots * (team->offensiveRating / oppDef) * (oppGk / 88.0) * venueMult;
	std::normal_distribution<double> dist(lambda, 5.5);
	double shots = dist(rng);
	return static_cast<int>(std::clamp(shots, 12.0, 52.0));
}

bool League::shotIsSaved(int shooterOffStrength, int goalieStrength, int oppDefStrength, bool homeShooter, double venueMult, double k) {
	std::mt19937 rng{ std::random_device{}() };
	double effectiveResistance = 0.65 * goalieStrength + 0.35 * oppDefStrength;
	double adjustedOff = shooterOffStrength * (1.0 + (venueMult -1.0) * 0.15);
	double diff = adjustedOff - effectiveResistance;
	double saveProbability = 1.0 / (1.0 + std::exp(-k * diff));
	saveProbability = std::clamp(saveProbability, 0.82, 0.97);
	thread_local std::uniform_real_distribution<double> ud(0.0, 1.0);
	return ud(rng) < saveProbability;
}

void League::goalScoredBy(std::shared_ptr<Team> team, bool playoffs) {
	std::mt19937 rng{ std::random_device{}() };
	// Build weight vector from roster (skaters get their offense as weight, goalies get small weight)
	std::vector<double> weights;
	weights.reserve(team->roster.size());
	for (const auto& p : team->roster) {
		if (auto* sk = dynamic_cast<Skater*>(p.get()))
			weights.push_back(static_cast<double>(sk->Offense()));
		else
			weights.push_back(1.0); // goalie or unknown player gets low weight
	}
	// Create distribution from weights and pick an index
	std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
	size_t idx = dist(rng);
	// idx is the selected player; update stats if it's a skater
	if (auto* scorer = dynamic_cast<Player*>(team->roster[idx].get())) {
		// Example stat update; adjust to your desired bookkeeping
		playoffs ? scorer->addPlayoffGoal() : scorer->addGoal();
		// You can also log or update assists, etc.
	}
	//Calculate assists, but give them less weight. For simplicity, flip a coin if goal was assisted on, then flip another coin to determine if there were 1 or 2 assists, then pick random players weighted by offense to assign assists to.
	bool assisted = std::bernoulli_distribution(0.83)(rng); //Roughly 80-85% of goals are typically assisted
	if (assisted) {
		int assistCount = std::bernoulli_distribution(0.94)(rng) ? 2 : 1; //Of the assisted goals, around 91-97% have 2 assists, the rest have 1 assist
		for (int i = 0; i < assistCount; ++i) {
			size_t assistIndex = dist(rng);
			if (auto* assister = dynamic_cast<Player*>(team->roster[assistIndex].get()))
				playoffs ? assister->addPlayoffAssist() : assister->addAssist();
		}
	}
}


void League::SimulateSeason() {
	//Clear things from previous season, if applicable
	finalStandings.clear();
	top3.clear();
	wildCards.clear();
	for (auto& team : teams)
		team->resetStats();
	//Loop through all the teams and make them play 42 home games as per NHL's new 84 schedule (in use from 2026-2027 onward)
	for (auto& team : teams) {
		auto opponents = getTeamsInSameDivision(*team);
		for (int i = 0; i < 7; ++i) {
			//Play each divisional opponent 3 times at home
			SimulateMatchup(team, opponents[i]);
			SimulateMatchup(team, opponents[i]);
			SimulateMatchup(team, opponents[i]);
		}
		opponents.clear();
		opponents = getTeamsInOtherDivision(*team);
		for (int i = 0; i < 7; ++i) {
			//Play each non-divisional conference opponent 2 times at home
			SimulateMatchup(team, opponents[i]);
			SimulateMatchup(team, opponents[i]);
		}
		opponents.clear();
		opponents = getTeamsInOtherConference(*team);
		for (int i = 0; i < 15; ++i) {
			//Play each inter-conference opponent once at home
			SimulateMatchup(team, opponents[i]);
		}
	}
	//This gives us a total of 7*3 + 7*2 + 15 = 50 home games per team, which is more than the 42 home games in NHL, but who cares?
	tallyPointsForAllTeams();
	setupPlayoffs();
}

void League::setupPlayoffs() {
	std::vector<std::shared_ptr<Team>> temp = teams;
	auto descending = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) -> bool {
		if (!a) return false;
		if (!b) return true;
		return a->points > b->points;
		};
	//Phase 1: Make groups of 8 and take the top 3 from each grouping
	std::vector<std::shared_ptr<Team>> remaining;
	std::size_t i = 0;
	while (i < temp.size()) {
		std::size_t groupEnd = std::min(i + 8, temp.size());
		std::size_t takeAmount = std::min<std::size_t>(3, groupEnd - i);
		if (takeAmount > 0 && groupEnd - i >= takeAmount) {
			std::partial_sort(temp.begin() + i,
							temp.begin() + i + takeAmount,
							temp.begin() + groupEnd,
							descending);
		}
		//Collect the top 3
		std::move(temp.begin() + i, temp.begin() + i + takeAmount, std::back_inserter(top3));
		//Keep the rest for phase 2
		std::move(temp.begin() + i + takeAmount, temp.begin() + groupEnd, std::back_inserter(remaining));
		i += 8;
	}
	//Phase 2: Take the remaining teams and sort them in groups as well
	//First, do partial sort for the remaining teams but separate by conference
	i = 0;
	while (i < remaining.size()) {
		std::size_t groupEnd = std::min(i + 10, remaining.size());
		std::size_t takeAmount = std::min<std::size_t>(2, groupEnd - i);
		if (takeAmount > 0 && groupEnd - i >= takeAmount) {
			std::partial_sort(remaining.begin() + i,
							remaining.begin() + i + takeAmount,
							remaining.begin() + groupEnd,
							descending);
		}
		std::move(remaining.begin() + i, remaining.begin() + i + takeAmount, std::back_inserter(wildCards));
		//Move the rest to final standings
		std::move(remaining.begin() + i + takeAmount, remaining.begin() + groupEnd, std::back_inserter(finalStandings));
		i += 10;
	}
	std::sort(finalStandings.begin(), finalStandings.end(), descending);
	printStandings();
	//Reverse the order of the final standings so that the worst team is at the top for easier drafting later
	auto ascending = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) -> bool {
		if (!a) return false;
		if (!b) return true;
		return a->points < b->points;
		};
	std::sort(finalStandings.begin(), finalStandings.end(), ascending);
}

void League::simulatePlayoffSeries(std::shared_ptr<Team> highSeed, std::shared_ptr<Team> lowSeed) {
	if(!highSeed || !lowSeed)
		throw std::invalid_argument("Both teams in a playoff series must be valid");

	int highSeedWins = 0;
	int lowSeedWins = 0;

	auto simulateGame = [&](bool highSeedIsHome) {
		std::shared_ptr<Team> home = highSeedIsHome ? highSeed : lowSeed;
		std::shared_ptr<Team> away = highSeedIsHome ? lowSeed : highSeed;
		std::shared_ptr<Team> winner = SimulateMatchup(home, away, 28.0, true);
		if (winner->abbreviation == highSeed->abbreviation)
			highSeedWins++;
		else
			lowSeedWins++;
	};
	simulateGame(true);
	simulateGame(true);
	simulateGame(false);
	simulateGame(false);
	if (highSeedWins == 4 || lowSeedWins == 4)goto done;
	simulateGame(true);
	if (highSeedWins == 4 || lowSeedWins == 4)goto done;
	simulateGame(false);
	if (highSeedWins == 4 || lowSeedWins == 4)goto done;
	simulateGame(true);//Game seven
	
done:
	if (highSeedWins == 4) {
		playoffWinners.push_back(std::move(highSeed));
		playoffLosers.push_back(std::move(lowSeed));
	}
	else if(lowSeedWins == 4) {
		playoffWinners.push_back(std::move(lowSeed));
		playoffLosers.push_back(std::move(highSeed));
	}
	else {
		throw std::logic_error("A playoff series should have a winner after 7 games");
	}
}

std::shared_ptr<Team> League::SimulateMatchup(std::shared_ptr<Team> home, std::shared_ptr<Team> away, double baseShots, bool playoffs) {
	std::mt19937 rng{ std::random_device{}() };
	int homeGk = home->selectStartingGoalie().Goaltending();
	int awayGk = away->selectStartingGoalie().Goaltending();
	int shotsHome = sampleShots(home, true, baseShots, away->defensiveRating, awayGk);
	int shotsAway = sampleShots(away, false, baseShots, home->defensiveRating, homeGk);
	int goalsHome = 0, goalsAway = 0;
	for (int i = 0; i < shotsHome; ++i) {
		if (!shotIsSaved(home->offensiveRating, awayGk, away->defensiveRating, true, home->homeAdv)) {
			goalsHome++;
			goalScoredBy(home,playoffs);
		}
	}
	for (int i = 0; i < shotsAway; ++i) {
		if (!shotIsSaved(away->offensiveRating, homeGk, home->defensiveRating, false, away->awayAdv)) {
			goalsAway++;
			goalScoredBy(away,playoffs);
		}
	}
	//Overtime?
	bool OT = false;
	if (goalsHome == goalsAway) {
		OT = true;
		shotsHome += sampleShots(home, true, 16, away->defensiveRating, awayGk);
		shotsAway += sampleShots(away, false, 16, home->defensiveRating, homeGk);
		thread_local std::uniform_int_distribution<int> random(0,1);
		random(rng) == 1 ? goalsHome++ : goalsAway++;
		if (random(rng) == 1) {
			goalsHome++;
			goalScoredBy(home, playoffs);
		}
		else {
			goalsAway++;
			goalScoredBy(away, playoffs);
		}
	}
	if (!playoffs) {
		std::cout << "Score: " << away->abbreviation << goalsAway << " - " << goalsHome << home->abbreviation << "\n";
		if (goalsHome > goalsAway) {
			home->wins++;
			OT ? away->otl++ : away->losses++;
		}
		else {
			away->wins++;
			OT ? home->otl++ : home->losses++;
		}
	}

	updateVenueMultipliers(home, true, shotsHome, goalsHome, shotsAway, goalsAway);
	updateVenueMultipliers(away, false, shotsAway, goalsAway, shotsHome, goalsHome);

	return (goalsHome >= goalsAway) ? home : away;
}

void League::simulatePlayoffsFirstRound() {
	//Division winners
	std::shared_ptr<Team> bestEast = betterSeed(top3[0], top3[3]) == top3[0] ? top3[0] : top3[3]; //Plays against WC2
	std::shared_ptr<Team> secondBestEast = betterSeed(top3[0], top3[3]) == top3[0] ? top3[3] : top3[0];//Plays against WC1
	std::shared_ptr<Team> bestWest = betterSeed(top3[6], top3[9]) == top3[6] ? top3[6] : top3[9];//Plays against WC4
	std::shared_ptr<Team> secondBestWest = betterSeed(top3[6], top3[9]) == top3[6] ? top3[9] : top3[6];//Plays against WC3
	simulatePlayoffSeries(bestEast, wildCards[1]);
	simulatePlayoffSeries(top3[1], top3[2]);
	simulatePlayoffSeries(secondBestEast, wildCards[0]);
	simulatePlayoffSeries(top3[4], top3[5]);
	simulatePlayoffSeries(bestWest, wildCards[3]);
	simulatePlayoffSeries(top3[7], top3[8]);
	simulatePlayoffSeries(secondBestWest, wildCards[2]);
	simulatePlayoffSeries(top3[10], top3[11]);
	//Simulating east first means the eastern teams will be first in the playoffsWinners vector for ease of pairing
	
	auto ascending = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) -> bool {
		if (!a) return false;
		if (!b) return true;
		return a->points < b->points;
		};
	//Sort the losers by points so that the highest ranked losers are higher in the final standings
	std::sort(playoffLosers.begin(), playoffLosers.end(), ascending);

	for (auto loser : playoffLosers) {
		std::cout << "Eliminated: " << loser->name << "\n";
		finalStandings.push_back(loser);
	}
}

void League::simulatePlayoffsSecondRound() {
	std::shared_ptr<Team> atl1 = playoffWinners[0];	//playoffWinners[0] is the winner of bestEast vs WC2
	std::shared_ptr<Team> atl2 = playoffWinners[1];	//playoffWinners[1] is the winner of top3[1] vs top3[2]
	std::shared_ptr<Team> met1 = playoffWinners[2];	//playoffWinners[2] is the winner of secondBestEast vs WC1
	std::shared_ptr<Team> met2 = playoffWinners[3];	//playoffWinners[3] is the winner of top3[4] vs top3[5]
	std::shared_ptr<Team> cen1 = playoffWinners[4];	//playoffWinners[4] is the winner of bestWest vs WC4
	std::shared_ptr<Team> cen2 = playoffWinners[5];	//playoffWinners[5] is the winner of top3[7] vs top3[8]
	std::shared_ptr<Team> pac1 = playoffWinners[6];	//playoffWinners[6] is the winner of secondBestWest vs WC3
	std::shared_ptr<Team> pac2 = playoffWinners[7];	//playoffWinners[7] is the winner of top3[10] vs top3[11]

	playoffWinners.clear();
	playoffLosers.clear();

	std::shared_ptr<Team> higherAtl = betterSeed(atl1, atl2);
	if (higherAtl->abbreviation == atl1->abbreviation)
		simulatePlayoffSeries(atl1, atl2);
	else
		simulatePlayoffSeries(atl2, atl1);

	std::shared_ptr<Team> higherMet = betterSeed(met1, met2);
	if (higherMet->abbreviation == met1->abbreviation)
		simulatePlayoffSeries(met1, met2);
	else
		simulatePlayoffSeries(met2, met1);

	std::shared_ptr<Team> higherCen = betterSeed(cen1, cen2);
	if (higherCen->abbreviation == cen1->abbreviation)
		simulatePlayoffSeries(cen1, cen2);
	else
		simulatePlayoffSeries(cen2, cen1);

	std::shared_ptr<Team> higherPac = betterSeed(pac1, pac2);
	if (higherPac->abbreviation == pac1->abbreviation)
		simulatePlayoffSeries(pac1, pac2);
	else
		simulatePlayoffSeries(pac2, pac1);

	auto ascending = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) -> bool {
		if (!a) return false;
		if (!b) return true;
		return a->points < b->points;
		};

	std::sort(playoffLosers.begin(), playoffLosers.end(), ascending);

	for (auto loser : playoffLosers) {
		std::cout << "Eliminated: " << loser->name << "\n";
		finalStandings.push_back(loser);
	}
}

void League::simulatePlayoffsConferenceFinals() {
	auto betterSeed = [](std::shared_ptr<Team> a, std::shared_ptr<Team> b) -> std::shared_ptr<Team> {
		if (a->points != b->points) {
			return a->points > b->points ? a : b;
		}
		return a;
	};
	std::shared_ptr<Team> atlWinner = playoffWinners[0]; //Winner of ATL division
	std::shared_ptr<Team> metWinner = playoffWinners[1]; //Winner of MET division
	std::shared_ptr<Team> cenWinner = playoffWinners[2]; //Winner of CEN division
	std::shared_ptr<Team> pacWinner = playoffWinners[3]; //Winner of PAC division
	playoffWinners.clear();
	playoffLosers.clear();
	//Determine which team is higher seed in each matchup. Seeding is determined by points
	std::shared_ptr<Team> higherEast = betterSeed(atlWinner,metWinner);
	if(higherEast->abbreviation == atlWinner->abbreviation)
		simulatePlayoffSeries(atlWinner, metWinner);
	else
		simulatePlayoffSeries(metWinner, atlWinner);
	std::shared_ptr<Team> higherWest = betterSeed(cenWinner, pacWinner);
	if(higherWest->abbreviation == cenWinner->abbreviation)
		simulatePlayoffSeries(cenWinner, pacWinner);
	else
		simulatePlayoffSeries(pacWinner, cenWinner);

	auto ascending = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) -> bool {
		if (!a) return false;
		if (!b) return true;
		return a->points < b->points;
		};

	std::sort(playoffLosers.begin(), playoffLosers.end(), ascending);

	for (auto loser : playoffLosers) {
		std::cout << "Eliminated: " << loser->name << "\n";
		finalStandings.push_back(loser);
	}
}

void League::simulatePlayoffsStanleyCupFinals() {
	std::shared_ptr<Team> finalist1 = playoffWinners[0]; //Winner of Eastern Conference
	std::shared_ptr<Team> finalist2 = playoffWinners[1]; //Winner of Western Conference
	playoffWinners.clear();
	playoffLosers.clear();
	std::shared_ptr<Team> higherSeed = betterSeed(finalist1, finalist2);
	if(higherSeed->abbreviation == finalist1->abbreviation)
		simulatePlayoffSeries(finalist1, finalist2);
	else
		simulatePlayoffSeries(finalist2, finalist1);

	//After this, we should have our champion and runner-up in playoffWinners and playoffLosers respectively.
	//Push both to final standings in the correct order
	finalStandings.push_back(playoffLosers[0]);
	finalStandings.push_back(playoffWinners[0]);
}

void League::updateVenueMultipliers(std::shared_ptr<Team> team, bool wasHome, int shotsFor, int goalsFor, int shotsAgainst, int goalsAgainst, double learningRate) {
	double performance = (goalsFor - goalsAgainst) + 0.15 * (shotsFor - shotsAgainst);
	performance = std::clamp(performance / 3.0, -2.0, 2.0);
	if (wasHome) {
		team->homeAdv += learningRate * (performance - (team->homeAdv - 1.0));
		team->homeAdv = std::clamp(team->homeAdv, 0.75, 1.25);
	}
	else {
		team->awayAdv += learningRate * (performance - (team->awayAdv - 1.0));
		team->awayAdv = std::clamp(team->awayAdv, 0.75, 1.25);
	}
}

