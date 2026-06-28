#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Item.h"
#include <json.hpp> //nlohmann/json library

class ItemDatabase {
private:
	std::unordered_map<std::string, std::unique_ptr<Item>> _prototypes;
	std::unique_ptr<Item> createItemFromJson(const std::string& id, const nlohmann::json& data);

public:
	bool loadFromJson(const std::string& filename);

	std::unique_ptr<Item> createItem(const std::string& itemID) const;//Creates an item from the prototype map
	const Item* getPrototype(const std::string& itemID) const; //An utility function for looking up item data without creating an instance

	StatModifiers parseStatModifiers(const nlohmann::json& jsonMods);
	
	ItemType getItemTypeFromString(const std::string& type) const;
	EquipmentSlot getEquipmentSlotFromString(const std::string& slot) const;
	CombatStat getCombatStatFromString(const std::string& stat) const;
};