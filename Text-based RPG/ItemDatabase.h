#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include "Item.h"
#include <json.hpp> //nlohmann/json library

class ItemDatabase {
private:
	std::unordered_map<std::string, std::shared_ptr<Item>> _prototypes;
	std::shared_ptr<Item> createItemFromJson(const std::string& id, const nlohmann::json& data);

	const std::unordered_map<std::string, ItemType> stringToItemTypeMap = {
		{"weapon", ItemType::WEAPON},
		{"armor", ItemType::ARMOR},
		{"consumable", ItemType::CONSUMABLE},
		{"trinket", ItemType::TRINKET},
		{"currency", ItemType::CURRENCY},
		{"key", ItemType::KEY},
		{"keyitem", ItemType::KEYITEM},
		{"unknown", ItemType::UNKNOWN}
	};

	const std::unordered_map<std::string, EquipmentSlot> stringToEquipmentSlotMap = {
		{"head", EquipmentSlot::HEAD},
		{"chest", EquipmentSlot::CHEST},
		{"legs", EquipmentSlot::LEGS},
		{"body", EquipmentSlot::BODY},
		{"feet", EquipmentSlot::FEET},
		{"arms", EquipmentSlot::ARMS},
		{"hand", EquipmentSlot::HAND},
		{"ring", EquipmentSlot::RING},
		{"amulet", EquipmentSlot::AMULET},
		{"none", EquipmentSlot::NONE}
	};

	const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
		{"hp", CombatStat::HP},
		{"maxhp", CombatStat::MAXHP},
		{"mp", CombatStat::MP},
		{"maxmp", CombatStat::MAXMP},
		{"power", CombatStat::POWER},
		{"fortitude", CombatStat::FORTITUDE},
		{"sorcery", CombatStat::SORCERY},
		{"willpower", CombatStat::WILLPOWER},
		{"speed", CombatStat::SPEED},
		{"luck", CombatStat::LUCK}
	};

public:
	bool loadFromJson(const std::string& filename);

	std::shared_ptr<Item> createItem(const std::string& itemID) const;
	const Item* getPrototype(const std::string& itemID) const;

	StatModifiers parseStatModifiers(const nlohmann::json& jsonMods);
	
	ItemType getItemTypeFromString(const std::string& type) const;
	EquipmentSlot getEquipmentSlotFromString(const std::string& slot) const;
	CombatStat getCombatStatFromString(const std::string& stat) const;
};