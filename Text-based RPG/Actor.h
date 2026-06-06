#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <iostream>
#include "Stats.h"
#include "Inventory.h"

class Actor{
protected:
	std::string _name;
	std::string _description;

	virtual bool equals(const Actor& other) const {
		const auto& a = static_cast<const Actor&>(other);
		return _name == a._name;
	}

public:
	const std::string ID;
	Actor(std::string id, const std::string& name, const std::string description) : ID(id), _name(name), _description(description) {}
	virtual ~Actor() = default;
	Actor(const Actor&) = default;
	Actor(Actor&&) = default;
	Actor& operator=(const Actor&) = default;
	Actor& operator=(Actor&&) = default;
	bool operator==(const Actor& other) const {
		return typeid(*this) == typeid(other) && equals(other);
	}
	bool operator!=(const Actor& other) const{
		return !(*this == other);
	}

	std::string GetName() const { return _name; }

	virtual std::unique_ptr<Actor> clone() const = 0;
};

class NPC : public Actor {
protected:
	std::vector<std::string> _dialogue;
	
	bool equals(const Actor& other) const override {
		if (!Actor::equals(other)) return false;
		const auto& n = static_cast<const NPC&>(other);
		return _dialogue == n._dialogue;
	}
public:
	NPC(std::string id, const std::string& name, const std::string description, std::vector<std::string>& dialogue) : Actor(id, name, description), _dialogue(std::move(dialogue)) {}
	std::unique_ptr<Actor> clone() const override {
		return std::make_unique<NPC>(*this);
	}
	std::vector<std::string> GetDialogue() const { return _dialogue; }
};

class Enemy : public Actor {
private:
	ActorStats _stats;
	Inventory _loot;
	std::vector<float> _lootDropRates{};
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> _equipped{};

	bool equals(const Actor& other) const override {
		if (!Actor::equals(other)) return false;
		const auto& e = static_cast<const Enemy&>(other);
		return _stats.baseStats == e._stats.baseStats && _stats.growthRates == e._stats.growthRates && _loot == e._loot && _lootDropRates == e._lootDropRates && _equipped == e._equipped;
	}
public:
	Enemy(std::string id, const std::string& name, const std::string description, ActorStats stats, Inventory loot, std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> equipped) : Actor(id, name, description), _stats(stats), _loot(loot), _equipped(equipped) {}
	ActorStats& GetStats() { return _stats; }
	const ActorStats& GetStats() const { return _stats; }
	Inventory& GetLoot() { return _loot; }
	const Inventory& GetLoot() const { return _loot; }
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> GetEquipped() const { return _equipped; }

	std::unique_ptr<Actor> clone() const override {
		return std::make_unique<Enemy>(*this);
	}

	void physicalAttack(Player& player) {
		int enemyPower = _stats.baseStats[(size_t)CombatStat::POWER];
		float multiplier = 1.0f;
		for (auto& e : _equipped) {
			if (e) {
				enemyPower += e->getModifiers().flatModifiers[(size_t)CombatStat::POWER];
				multiplier += e->getModifiers().multipliers[(size_t)CombatStat::POWER];
			}
		}
		enemyPower = static_cast<int>(enemyPower * multiplier);

		int playerDefense = player.GetStats().baseStats[(size_t)CombatStat::FORTITUDE];
		float defMultiplier = 1.0f;
		for (auto& e : _equipped) {
			if (e) {
				playerDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::FORTITUDE];
				defMultiplier += e->getModifiers().multipliers[(size_t)CombatStat::FORTITUDE];
			}
		}
		playerDefense = static_cast<int>(playerDefense * defMultiplier);

		int damage = playerDefense - enemyPower;
		if (damage < 0)
			damage = 0;
		player.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("{} attacked, doing {} points of damage!", _name, damage);
	}

	void magicAttack(Player& player) {
		int enemyPower = _stats.baseStats[(size_t)CombatStat::SORCERY];
		float multiplier = 1.0f;
		for (auto& e : _equipped) {
			if (e) {
				enemyPower += e->getModifiers().flatModifiers[(size_t)CombatStat::SORCERY];
				multiplier += e->getModifiers().multipliers[(size_t)CombatStat::SORCERY];
			}
		}
		enemyPower = static_cast<int>(enemyPower * multiplier);
		int playerDefense = player.GetStats().baseStats[(size_t)CombatStat::WILLPOWER];
		float defMultiplier = 1.0f;
		for (auto& e : _equipped) {
			if (e) {
				playerDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::WILLPOWER];
				defMultiplier += e->getModifiers().multipliers[(size_t)CombatStat::WILLPOWER];
			}
		}
		playerDefense = static_cast<int>(playerDefense * defMultiplier);

		int damage = playerDefense - enemyPower;
		if (damage < 0)
			damage = 0;
		player.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("{} sorcered, doing {} points of damage!", _name, damage);
	}
};

class Merchant : public Actor {
private:
	Inventory _inventory;
public:
	Merchant(std::string id, const std::string& name, const std::string description, Inventory inventory) : Actor(id, name, description), _inventory(inventory) {}
	Inventory GetInventory() const { return _inventory; }
	std::unique_ptr<Actor> clone() const override {
		return std::make_unique<Merchant>(*this);
	}
};

//For chests, barrels, cupboards, etc. Even though I'm probably just going to use chests only.
class Container : public Actor {
private:
		Inventory _inventory;
public:
	Container(std::string id, const std::string& name, const std::string description, Inventory inventory) : Actor(id, name, description), _inventory(inventory) {}
	std::unique_ptr<Actor> clone() const override {
		return std::make_unique<Container>(*this);
	}
	Inventory& GetInventory() { return _inventory; }
	const Inventory& GetInventory() const { return _inventory; }

	void open() {
		std::println("You open {} and find:", _name);
		for (const auto& stack : _inventory.getStacks())
			std::println("- {} x{}", stack.second._item->getName(), stack.second._quantity);
	}
};