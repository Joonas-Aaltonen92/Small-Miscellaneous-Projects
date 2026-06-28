#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Stats.h"
#include <json.hpp> //nlohmann/json library

class Actor;
class ItemDatabase;

class ActorDatabase {
private:
	std::unordered_map<std::string, std::unique_ptr<Actor>> _prototypes;
	std::unique_ptr<Actor> createActorFromJson(const std::string& id, const nlohmann::json& data);

	ItemDatabase& database;

public:
	ActorDatabase(ItemDatabase& db) : database(db) {};
	bool loadFromJson(const std::string& fileName);
	
	std::unique_ptr<Actor> createActor(const std::string& actorID) const;
	const Actor* getPrototype(const std::string& actorID) const;

	ActorStats parseActorStats(const nlohmann::json& jsonStats);
	CombatStat getCombatStatFromString(const std::string& combatStat) const;
	GrowthStat getGrowthStatFromString(const std::string& growthStat) const;
};