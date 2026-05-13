#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "Team.h"


class League {
	std::vector<std::string> conferences = {"EASTERN, WESTERN"};
	std::vector<std::string> divisions = {"ATLANTIC, METROPOLITAN, CENTRAL, PACIFIC"};
	std::vector<std::shared_ptr<Team>> teams;
	std::vector<std::shared_ptr<Team>> top3;
	std::vector<std::shared_ptr<Team>> wildCards;
	std::vector<std::shared_ptr<Team>> finalStandings;
	std::vector<std::shared_ptr<Team>> playoffWinners;
	std::vector<std::shared_ptr<Team>> playoffLosers;
	std::vector<std::shared_ptr<Player>> allPlayers;

	std::vector<std::shared_ptr<Player>> draftPool;

public:
	League();
	//Helper
	static const std::shared_ptr<Team>& betterSeed(const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) {
		if (!a) return b;
		if (!b) return a;
		return (a->points >= b->points) ? a : b;
	}

	int  sampleShots(std::shared_ptr<Team> team, bool isHome, double baseShots, int oppDef, int oppGk);
	bool shotIsSaved(int shooterOffStrength, int goalieStrength, int oppDefStrength, bool homeShooter, double venueMult, double k = 0.0085);
	void goalScoredBy(std::shared_ptr<Team> team, bool playoffs = false);
	std::shared_ptr<Team> SimulateMatchup(std::shared_ptr<Team> home, std::shared_ptr<Team> away, double baseShots = 28.0, bool playoffs = false);
	void SimulateSeason();

	void updateVenueMultipliers(std::shared_ptr<Team> team, bool wasHome, int shotsFor, int goalsFor, int shotsAgainst, int goalsAgainst, double learningRate = 0.08);

	void tallyPoints(Team& team) {
		team.points += (team.wins * 2 + team.otl);
	}

	void tallyPointsForAllTeams() {
		for (auto& team : teams) {
			tallyPoints(*team);
		}
	}

	//Arrange teams by division and wild card
	void setupPlayoffs();

	//Playoffs
	void simulatePlayoffs() {
		simulatePlayoffsFirstRound();
		simulatePlayoffsSecondRound();
		simulatePlayoffsConferenceFinals();
		simulatePlayoffsStanleyCupFinals();
	}
	void simulatePlayoffsFirstRound();
	void simulatePlayoffsSecondRound();
	void simulatePlayoffsConferenceFinals();
	void simulatePlayoffsStanleyCupFinals();
	void simulatePlayoffSeries(std::shared_ptr<Team> highSeed, std::shared_ptr<Team> lowSeed);

	//Increase ages, draft seven players in final standings order, and possibly replace old and busted players with better ones.
	void endSeason() {
		for(auto& team : teams)
			team->ageUp();

		generateDraftPool();
		draftPlayers();
		for(auto& team : teams)
			team->replacePlayers();
	}

	//Generate prospects and sort them into a draft pool
	void generateDraftPool() {
		draftPool.clear();//Should be cleared already but just in case
		std::mt19937 rng(std::random_device{}());
		auto nameMap = NameGenerator::GetInstance().nameByNationality;
		//Create forwards—the most common type of prospect (6/10 players from the total 32*7 prospects will be forwards)
		//That means roughly 135 out of the 224 players in the draft pool will be forwards
		for (int i = 0; i < 135; ++i) {
			auto it = nameMap.begin();
			std::advance(it, std::uniform_int_distribution<size_t>(0, nameMap.size() - 1)(rng));
			draftPool.emplace_back(std::make_shared<Skater>(it->first, "Offense", true));
		}
		//Create defensemen (3/10 prospects = rougly 67 players)
		for (int i = 0; i < 67; ++i) {
			auto it = nameMap.begin();
			std::advance(it, std::uniform_int_distribution<size_t>(0, nameMap.size() - 1)(rng));
			draftPool.emplace_back(std::make_shared<Skater>(it->first, "Defense", true));
		}
		//And last, 22 goalies (1/10 prospects)
		for (int i = 0; i < 22; ++i) {
			auto it = nameMap.begin();
			std::advance(it, std::uniform_int_distribution<size_t>(0, nameMap.size() - 1)(rng));
			draftPool.emplace_back(std::make_shared<Goalie>(it->first, "Goalie", true));
		}

		//Sort draft pool by overall rating so that the best prospects are at the front of the vector. Overall rating is determined by offense for offensive minded players, defense for defensive minded players, and goaltending for goalies.
		std::sort(draftPool.begin(), draftPool.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
			return a->getPlayerScore() > b->getPlayerScore();
		});
	}
	//Draft players in order of final standings, starting with the loser of losers (finalStandings[0]) and ending with the Stanley Cup champion (finalStandings[31])
	void draftPlayers() {
		//Clear team drafted players from previous season, if applicable
		for(auto& team : teams)
			team->emptyDraftPool();
		for (size_t i = 0; i < teams.size(); ++i) {
			auto& team = finalStandings[i];
			for (int j = 0; j < 7; ++j) {
				if (!draftPool.empty()) {
					team->draftedPlayers.push_back(draftPool.front());//Take the best available prospect
					draftPool.erase(draftPool.begin());//Remove the drafted player from the pool
				}
			}
		}
	}

	//Opponent helpers for scheduling and playoff matchups
	std::vector<std::shared_ptr<Team>> getTeamsInSameDivision(const Team& team) {
		std::vector<std::shared_ptr<Team>> result;
		for (auto& t : teams) {
			if(!(t.get()->abbreviation == team.abbreviation) && t.get()->divisionID == team.divisionID)
				result.push_back(t);
		}
		return result;
	}

	std::vector<std::shared_ptr<Team>> getTeamsInOtherDivision(const Team& team) {
		std::vector<std::shared_ptr<Team>> result;
		for (auto& t : teams) {
			if (!(t.get()->abbreviation == team.abbreviation) && t.get()->conferenceID == team.conferenceID && t.get()->divisionID != team.divisionID)
				result.push_back(t);
		}
		return result;
	}

	std::vector<std::shared_ptr<Team>> getTeamsInOtherConference(const Team& team) {
		std::vector<std::shared_ptr<Team>> result;
		for (auto& t : teams) {
			if (!(t.get()->abbreviation == team.abbreviation) && t.get()->conferenceID != team.conferenceID)
				result.push_back(t);
		}
		return result;
	}


	//Print functions
	void printStandings() {
		//Print the standings in a nice format by division and wild card
		std::cout << "\n\n\n------------------FINAL STANDINGS-----------------------\n\n\n";
		std::cout << "ATLANTIC DIVISION:\n";
		for (int i = 0; i < 3; ++i) {
			std::cout << top3[i]->name << " - " << top3[i]->points << " points\n";
		}
		std::cout << "\n\nMETROPOLITAN DIVISION:\n";
		for (int i = 3; i < 6; ++i) {
			std::cout << top3[i]->name << " - " << top3[i]->points << " points\n";
		}
		std::cout << "\nWild Cards:\n";
		for (int i = 0; i < 2; ++i) {
			std::cout << wildCards[i]->name << " - " << wildCards[i]->points << " points\n";
		}
		std::cout << "\n\nCENTRAL DIVISION:\n";
		for (int i = 6; i < 9; ++i) {
			std::cout << top3[i]->name << " - " << top3[i]->points << " points\n";
		}
		std::cout << "\n\nPACIFIC DIVISION:\n";
		for (int i = 9; i < 12; ++i) {
			std::cout << top3[i]->name << " - " << top3[i]->points << " points\n";
		}
		std::cout << "\nWild Cards:\n";
		for (int i = 2; i < 4; ++i) {
			std::cout << wildCards[i]->name << " - " << wildCards[i]->points << " points\n";
		}

		std::cout << "\n\nRemaining Teams:\n";
		for (const auto& team : finalStandings) {
			std::cout << team->name << " - " << team->points << " points\n";
		}
	}

	void printFinalStandings() {
		std::cout << "\n\n\n------------------FINAL STANDINGS-----------------------\n\n\n";
		for (size_t i = 0; i < finalStandings.size(); ++i) {
			std::cout << (i + 1) << ". " << finalStandings[i]->name << " - " << finalStandings[i]->points << " points\n";
		}

		std::cout << "\n\nStanley Cup Champion: " << playoffWinners[0]->name << "!\n\n";
	}
	void collectAllPlayers() {
		allPlayers.clear();
		allPlayers.reserve(32 * 20);

		for (const auto& team : teams) {
			for (const auto& player : team->roster) {
				allPlayers.push_back(player);
			}
		}
	}

	void printTopScorersSeason(int topN = 20) {
		std::vector<std::shared_ptr<Player>> sortedPlayers = allPlayers;
		std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
			return a->Points() > b->Points();
			});

		std::cout << "\n\n\n------------------TOP SCORERS-----------------------\n\n\n";
		for (int i = 0; i < std::min(topN, static_cast<int>(sortedPlayers.size())); ++i) {
			const auto& player = sortedPlayers[i];
			std::cout << (i + 1) << ". " << player->getName().fullName() << " - " << player->Goals() << "G , " << player->Assists() << "A, " << player->Points() << "P\n";
		}
		std::cout << "\n\n";//Just some padding
	}

	void printTopScorersPlayoffs(int topN = 20) {
		std::vector<std::shared_ptr<Player>> sortedPlayers = allPlayers;
		std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
			return a->PlayoffPoints() > b->PlayoffPoints();
			});

		std::cout << "\n\n\n------------------TOP SCORERS IN PLAYOFFS-----------------------\n\n\n";
		for (int i = 0; i < std::min(topN, static_cast<int>(sortedPlayers.size())); ++i) {
			const auto& player = sortedPlayers[i];
			std::cout << (i + 1) << ". " << player->getName().fullName() << " - " << player->PlayoffGoals() << "G , " << player->PlayoffAssists() << "A, " << player->PlayoffPoints() << "P\n";
		}
		std::cout << "\n\n";//Just some padding
	}
};