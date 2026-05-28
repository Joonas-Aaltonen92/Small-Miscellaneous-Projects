#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Actor.h"
#include <json.hpp> //nlohmann/json library

class ActorDatabase {
private:
	std::unordered_map<std::string, std::shared_ptr<Actor>> _actors;
	std::shared_ptr<Actor> createActorFromJson(const std::string& id, const nlohmann::json& data);


};