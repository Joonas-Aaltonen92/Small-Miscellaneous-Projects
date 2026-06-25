#include "ActorDatabase.h"
#include "Actor.h"

namespace {
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
}

std::unique_ptr<Actor> ActorDatabase::createActorFromJson(const std::string& id, const nlohmann::json& data) {
	std::string type = data.value("type", "unknown");
	std::string name = data.value("name", "unknown");
	std::string description = data.value("description", "");


	if (type == "npc") {
		std::vector<std::string> dialogue = data["dialogue"];
		return std::make_unique<NPC>(id, name, description, dialogue);
	}
	if (type == "merchant") {
		
	}
	return nullptr;
}