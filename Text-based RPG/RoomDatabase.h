#pragma once
#include <string>
#include <unordered_map>
#include "RoomDefinition.h"

class RoomDatabase {
private:
	std::unordered_map<std::string, RoomDefinition> _rooms;
public:
	bool loadFromJson(const std::string& filename);
	bool saveToJson(const std::string& filename);
	const RoomDefinition* find(const std::string& id) const;
};