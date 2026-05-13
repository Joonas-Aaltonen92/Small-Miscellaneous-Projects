#pragma once
#include <vector>
#include <string>
#include <random>
#include "Player.h"

struct Team {
	std::string name;
	std::string abbreviation;
	int offensiveRating = 0;
	int defensiveRating = 0;
	int goalieRating = 0;
	int points = 0;
	int wins = 0;
	int losses = 0;
	int otl = 0;

	double homeAdv = 1.05;
	double awayAdv = 0.95;

	//Rolling averages; Track recent form for smoother updates
	double recentHomeRecord = 0.0;
	double recentAwayRecord = 0.0;

	std::string divisionID;
	std::string conferenceID;
	std::vector<std::shared_ptr<Player>> roster;
	std::vector<std::shared_ptr<Player>> draftedPlayers;

	std::vector<std::shared_ptr<Player>> allForwards;
	std::vector<std::shared_ptr<Player>> allDefensemen;
	std::vector<std::shared_ptr<Player>> allGoalies;

	Goalie* starter = nullptr;
	Goalie* backup = nullptr;

	int playoffSeed = 0;

	Team(std::string Name, std::string Abbreviation, std::string div, std::string conf) :
		name(std::move(Name)),
		abbreviation(std::move(Abbreviation)),
		divisionID(std::move(div)),
		conferenceID(std::move(conf))
	{
		createRoster();
		calculateRatings();
	}

	Team(const Team&) = delete;
	Team& operator=(const Team&) = delete;

	Team(Team&&) = default;
	Team& operator=(Team&&) = default;

	bool operator==(const Team& other) {
		return (name == other.name);
	}

	void createRoster() {
		std::mt19937 rng(std::random_device{}());
		auto nameMap = NameGenerator::GetInstance().nameByNationality;
		//Create offensive skaters
		for (int i = 0; i < 12; ++i) {
			auto it = nameMap.begin();
			std::advance(it, std::uniform_int_distribution<size_t>(0, nameMap.size() - 1)(rng));
			roster.emplace_back(std::make_shared<Skater>(it->first, "Offense",false));
		}
		//Create defensemen
		for (int i = 0; i < 6; ++i) {
			auto it = nameMap.begin();
			std::advance(it, std::uniform_int_distribution<size_t>(0, nameMap.size() - 1)(rng));
			roster.emplace_back(std::make_shared<Skater>(it->first, "Defense",false));
		}
		//Make goalies
		for (int i = 0; i < 2; ++i) {
			auto it = nameMap.begin();
			std::advance(it, std::uniform_int_distribution<size_t>(0, nameMap.size() - 1)(rng));
			roster.emplace_back(std::make_shared<Goalie>(it->first, "Goalie",false));
		}
	}

	void calculateRatings() {
		offensiveRating = 0;
		defensiveRating = 0;
		for (const auto& p : roster) {
			if (auto* skater = dynamic_cast<Skater*>(p.get())) {
				offensiveRating += skater->Offense();
				defensiveRating += skater->Defense();
			}
			else if (auto* goalie = dynamic_cast<Goalie*>(p.get())) {
				if (starter == nullptr) {
					starter = goalie;
					backup = goalie;
				}
				else if (goalie->Goaltending() > starter->Goaltending()) {
					backup = starter;
					starter = goalie;
				}
				else {
					backup = goalie;
				}
				goalieRating += goalie->Goaltending();
			}
		}
		if (!roster.empty()) {
			offensiveRating /= 18;
			defensiveRating /= 18;
			goalieRating /= 2;
		}
	}

	Goalie& selectStartingGoalie(double starterBias = 0.68) const {
		//startingBias = probability of starting goalie being the starter, as opposed to backup.
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<double>dist(0.0, 1.0);
		if(dist(rng) < starterBias)
			return *starter;
		else
			return *backup;
	}

	//Go through all players, age them up and change stats based on age
	void ageUp() {
		for (const auto& player : roster) {
			player->increaseAge();
			if (auto* skater = dynamic_cast<Skater*>(player.get())) {
				//Forwards and defenders typically age differently—offensive rating peaks earlier than defense, and offense also has deeper decline
				//Helper lambda to check which is higer between offense and defense (this determines if player is more offense minded or defense minded)
				auto isOffensiveMinded = [](Skater* skater) -> bool {
					return skater->Offense() > skater->Defense();
					};
				skater->changeStat(isOffensiveMinded(skater));
			}
			else if(auto* goalie = dynamic_cast<Goalie*>(player.get())) {
				goalie->changeStat();
			}
		}
	}

	void emptyDraftPool() {
		draftedPlayers.clear();
	}

	//Replace old players with drafted ones, if drafted players are better in given role
	void replacePlayers() {
		//First move all roster and drafted forwards to allForwards
		allForwards.clear();
		for (const auto& player : roster) {
			if (auto* skater = dynamic_cast<Skater*>(player.get())) {
				if (skater->Role() == "Offense") {
					allForwards.push_back(player);
				}else if(skater->Role() == "Defense")
					allDefensemen.push_back(player);
			}
			if(auto* skater = dynamic_cast<Goalie*>(player.get()))
				allGoalies.push_back(player);
		}
		//Do the same with drafted players
		for (const auto& prospect : draftedPlayers) {
			if (auto* skater = dynamic_cast<Skater*>(prospect.get())) {
				if (skater->Role() == "Offense") {
					allForwards.push_back(prospect);
				}
				else if (skater->Role() == "Defense")
					allDefensemen.push_back(prospect);
			}
			if (auto* skater = dynamic_cast<Goalie*>(prospect.get()))
				allGoalies.push_back(prospect);
		}
		auto comparePlayers = [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
			return a->getPlayerScore() > b->getPlayerScore();
			};

		std::sort(allForwards.begin(), allForwards.end(), comparePlayers);
		std::sort(allDefensemen.begin(), allDefensemen.end(), comparePlayers);
		std::sort(allGoalies.begin(), allGoalies.end(), comparePlayers);

		roster.clear();
		//Fill roster with the best 12 offensive players, 6 defensive players, and 2 goalies from the combined pool of roster and drafted players
		roster.insert(roster.end(), allForwards.begin(), allForwards.size() >= 12 ? allForwards.begin() + 12 : allForwards.end());
		roster.insert(roster.end(), allDefensemen.begin(), allDefensemen.size() >= 6 ? allDefensemen.begin() + 6 : allDefensemen.end());
		roster.insert(roster.end(), allGoalies.begin(), allGoalies.size() >= 2 ? allGoalies.begin() + 2 : allGoalies.end());
	}

	void resetStats() {
		points = 0;
		wins = 0;
		losses = 0;
		otl = 0;

		for(auto& player : roster)
			player->resetSeasonStats();
	}
};