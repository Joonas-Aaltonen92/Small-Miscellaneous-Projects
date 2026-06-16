#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Stats.h"
#include <json.hpp> //nlohmann/json library

class Actor;

class ActorDatabase {
private:
	std::unordered_map<std::string, std::unique_ptr<Actor>> _prototypes;
	std::unique_ptr<Actor> createActorFromJson(const std::string& id, const nlohmann::json& data);

	const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
		{"hp",CombatStat::HP},
		{"maxhp", CombatStat::MAXHP},
		{"mp", CombatStat::MP},
		{"maxmp", CombatStat::MAXMP},
		{"power", CombatStat::POWER},
		{"fortitude", CombatStat::FORTITUDE},
		{"sorcery", CombatStat::SORCERY},
		{"willpower", CombatStat::WILLPOWER},
		{"speed", CombatStat::SPEED},
		{"luck", CombatStat::LUCK},
	};

	const std::unordered_map<std::string, GrowthStat> stringToGrowthStatMap = {
	 {"vitality", GrowthStat::VITALITY},
	 {"wisdom", GrowthStat::WISDOM},
	 {"strength", GrowthStat::STRENGTH},
	 {"endurance", GrowthStat::ENDURANCE},
	 {"intelligence", GrowthStat::INTELLIGENCE},
	 {"resolve", GrowthStat::RESOLVE},
	 {"agility", GrowthStat::AGILITY},
	 {"fate", GrowthStat::FATE}
	};


public:
	bool loadFromJson(const std::string& fileName);
	
	std::unique_ptr<Actor> createActor(const std::string& actorID);
	const Actor* getPrototype(const std::string& actorID) const;

	ActorStats parseActorStats(const nlohmann::json& jsonStats);

	CombatStat getCombatStatFromString(const std::string& combatStat) const;
	GrowthStat getGrowthStatFromString(const std::string& growthStat) const;


};