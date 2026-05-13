#pragma once
#include <random>
#include "FullName.h"
#include "NameGenerator.h"

class Player {
protected:
	std::string nationality;
	FullName name;
	std::string role;
	int age = 0;
	//Season stats (need to be reset)
	int goals= 0;
	int assists = 0;
	int playoffGoals = 0;
	int playoffAssists = 0;

	//Total stats (no need to reset)
	int totalGoals = 0;
	int totalAssists = 0;
	int totalPlayoffGoals = 0;
	int totalPlayoffAssists = 0;

public:
	Player(std::string nat, std::string r, bool prospect = false) : nationality(nat), role(r){
		name = NameGenerator::GetInstance().getRandomName(nat);
		std::mt19937 rng(std::random_device{}());
		prospect ? age = 18 : age = std::uniform_int_distribution<int>(20, 40)(rng);
	}
	Player(const Player&) = default;
	Player& operator=(const Player&) = default;

	virtual ~Player() = default;
	void addGoal() { goals += 1; }
	void addPlayoffGoal() { playoffGoals += 1; }
	void addPlayoffAssist() { playoffAssists += 1; }
	void addAssist() { assists += 1; }
	int Points() const { return goals + assists; }
	int PlayoffPoints() const { return playoffGoals + playoffAssists; }
	int Assists() const { return assists; }
	int PlayoffAssists() const { return playoffAssists; }
	int Goals() const { return goals; }
	int PlayoffGoals() const { return playoffGoals; }
	int TotalGoals() const { return totalGoals; }
	int TotalAssists() const { return totalAssists; }
	int TotalPlayoffGoals() const { return totalPlayoffGoals; }
	int TotalPlayoffAssists() const { return totalPlayoffAssists; }
	int TotalPoints() const { return totalGoals + totalAssists; }
	int TotalPlayoffPoints() const { return totalPlayoffGoals + totalPlayoffAssists; }
	FullName getName() const { return name; }
	std::string Role() const { return role; }
	void increaseAge() { age++; }
	virtual int getPlayerScore() const = 0; //Pure virtual function to get a player's overall prospect score for sorting in the draft pool

	void resetSeasonStats() {
		goals = 0;
		assists = 0;
		playoffGoals = 0;
		playoffAssists = 0;
	}
};

class Skater : public Player {
	int offensiveStat;
	int defensiveStat;

public:
	Skater(std::string nat, std::string r, bool prospect) :Player(nat,r,prospect) {
		std::mt19937 rng(std::random_device{}());
		int minVal = 10;
		int maxVal = 90;
		if(prospect)
			maxVal = 75;
		if (nat == "Finnish") //Give Finns a boost for no particular reason :D
			minVal = 50;
		offensiveStat = std::uniform_int_distribution<int>(minVal, maxVal)(rng);
		defensiveStat = std::uniform_int_distribution<int>(minVal, maxVal)(rng);
	}

	int Offense() { return offensiveStat; }
	int Defense() { return defensiveStat; }
	void changeStat(bool offensive) {
		if (offensive) {
			if (age <= 21) {
				offensiveStat += 2;
				return;
			}
			if (age <= 25) {
				offensiveStat += 3;
				defensiveStat += 1;
				return;
			}
			if (age <= 28) {
				offensiveStat += 4;
				defensiveStat += 1;
				return;
			}
			if (age <= 31) {
				offensiveStat -= 1;
				defensiveStat += 1;
				return;
			}
			//No else needed, just keep decreasing offense and inreasing defense as player gets older and slower
			offensiveStat -= 2;
			defensiveStat += 2;
		}
		else {//Defensive minded player
			if (age <= 21) {
				offensiveStat += 1;
				defensiveStat += 1;
				return;
			}
			if (age <= 25) {
				offensiveStat += 1;
				defensiveStat += 2;
				return;
			}
			if (age <= 28) {
				offensiveStat += 2;
				defensiveStat += 2;
				return;
			}
			if (age <= 31) {
				offensiveStat += 1;
				defensiveStat += 3;
				return;
			}
			if (age <= 35) {
				offensiveStat -= 1;
				defensiveStat -= 1;
				return;
			}
			offensiveStat -= 3;
			defensiveStat -= 2;
		}
	}
	int getPlayerScore() const override {
		//For skaters, overall prospect score is determined by offense and defense, weighted depending on role
		if(role == "Offense")
			return static_cast<int>(0.60 * offensiveStat + 0.40 * defensiveStat);
		else if(role == "Defense")
			return static_cast<int>(0.25 * offensiveStat + 0.75 * defensiveStat);
	}
};

class Goalie : public Player {
	int goalieStat;

public:
	Goalie(std::string nat, std::string r, bool prospect) : Player(nat,r, prospect) {
		std::mt19937 rng(std::random_device{}());
		int minVal = 20;
		int maxVal = 90;
		if(prospect)
			maxVal = 65;
		if (nat == "Finnish")
			minVal = 40;
		goalieStat = std::uniform_int_distribution<int>(minVal, maxVal)(rng);
	}

	int Goaltending() { return goalieStat; }
	void changeStat() {
		if (age <= 21) {
			goalieStat += 1;
			return;
		}
		if(age <= 25) {
			goalieStat += 2;
			return;
		}
		if(age<= 32) {
			goalieStat += 3;
			return;
		}
		//Goalies have pretty short peaks, so after 32 they start declining pretty fast
		goalieStat -= 5;
	}
	int getPlayerScore() const override {
		//For goalies, overall prospect score is determined solely by goaltending stat
		return goalieStat;
	}
};