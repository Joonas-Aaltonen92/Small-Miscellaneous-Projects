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

	virtual bool equals(const Actor& other) const {
		const auto& o = static_cast<const Actor&>(other);
		return _name == o._name;
	}

public:
	Actor(std::string name, Inventory inventory, ActorStats stats) : _name(name){}
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

	virtual std::shared_ptr<Actor> clone() const = 0;
};

class NPC : public Actor {
protected:
	std::vector<std::string> _dialogue;
	
	bool equals(const Actor& other) const override {
		if (!Actor::equals(other)) return false;
		const auto& o = static_cast<const NPC&>(other);
		return _dialogue == o._dialogue;
	}
public:
	NPC(std::string name, Inventory inventory, ActorStats stats, std::vector<std::string> dialogue) : Actor(name, inventory, stats), _dialogue(std::move(dialogue)) {}
	std::shared_ptr<Actor> clone() const override {
		return std::make_shared<NPC>(*this);
	}
	
	std::vector<std::string> GetDialogue() const { return _dialogue; }
};

class Enemy : public Actor {
private:
	ActorStats _stats;
	Inventory _loot;
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> _equipped{};
public:
	ActorStats& GetStats() { return _stats; }
	const ActorStats& GetStats() const { return _stats; }
	Inventory& GetLoot() { return _loot; }
	const Inventory& GetLoot() const { return _loot; }
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> GetEquipped() const { return _equipped; }

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

class Merchant : public NPC {
private:
	Inventory _inventory;
public:
	Inventory GetInventory() const { return _inventory; }
};

//For chests, barrels, cupboards, etc. Even though I'm probably just going to use chests only.
class Container : public Actor {
private:
		Inventory _inventory;
public:
	Inventory& GetInventory() { return _inventory; }
	const Inventory& GetInventory() const { return _inventory; }

	//Called when Container is created.
	void AddItem(std::shared_ptr<Item> item, int quantity = 1) {
		_inventory.addItem(std::move(item), quantity);
	}
};