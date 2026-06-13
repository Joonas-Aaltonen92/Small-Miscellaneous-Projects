#include "ActorDatabase.h"

std::unique_ptr<Actor> ActorDatabase::createActorFromJson(const std::string& id, const nlohmann::json& data) {
	std::string type = data.value("type", "unknown");
	std::string name = data.value("name", "unknown");
	std::string description = data.value("description", "");


	if (type == "npc") {
		std::vector<std::string> dialogue = data["dialogue"];
		return std::make_unique<NPC>(id, name, description, dialogue);
	}
	return nullptr;
}