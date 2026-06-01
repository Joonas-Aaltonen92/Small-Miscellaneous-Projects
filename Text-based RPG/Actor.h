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
		for (auto& e : _equipped) {
			if (e) {
				enemyPower += e->getModifiers().flatModifiers[(size_t)CombatStat::POWER];
				enemyPower = static_cast<int>(enemyPower * e->getModifiers().multipliers[(size_t)CombatStat::POWER]);
			}
		}
		int playerDefense = player.GetStats().baseStats[(size_t)CombatStat::FORTITUDE];
		for (auto& e : _equipped) {
			if (e) {
				playerDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::FORTITUDE];
				playerDefense = static_cast<int>(playerDefense * e->getModifiers().multipliers[(size_t)CombatStat::FORTITUDE]);
			}
		}

		int damage = playerDefense - enemyPower;
		if (damage < 0)
			damage = 0;
		player.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("{} attacked, doing {} points of damage!", _name, damage);
	}

	void magicAttack(Player& player) {
		int enemyPower = _stats.baseStats[(size_t)CombatStat::SORCERY];
		for (auto& e : _equipped) {
			if (e) {
				enemyPower += e->getModifiers().flatModifiers[(size_t)CombatStat::SORCERY];
				enemyPower = static_cast<int>(enemyPower * e->getModifiers().multipliers[(size_t)CombatStat::SORCERY]);
			}
		}
		int playerDefense = player.GetStats().baseStats[(size_t)CombatStat::WILLPOWER];
		for (auto& e : _equipped) {
			if (e) {
				playerDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::WILLPOWER];
				playerDefense = static_cast<int>(playerDefense * e->getModifiers().multipliers[(size_t)CombatStat::WILLPOWER]);
			}
		}

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



class Player : public Actor {
private:
	ActorStats _stats;
	Inventory _inventory;
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> _equipped{};
public:
	Player(std::string name, Inventory inventory, ActorStats stats, std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> equipped) : Actor(name, inventory, stats), _equipped(equipped) {}
	bool equals(const Actor& other) const override {
		if (!Actor::equals(other)) return false;
		const auto& o = static_cast<const Player&>(other);
		return _stats.baseStats == o._stats.baseStats && _stats.growthRates == o._stats.growthRates && _inventory == o._inventory && _equipped == o._equipped;
	}

	ActorStats& GetStats() { return _stats; }
	const ActorStats& GetStats() const { return _stats; }
	Inventory& GetInventory() { return _inventory; }
	const Inventory& GetInventory() const { return _inventory; }
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> GetEquipped() const { return _equipped; }

	void talk(NPC npc) {
		int counter = 0;
		while (counter < npc.GetDialogue().size()) {
			std::println("{}: {}", npc.GetName(), npc.GetDialogue()[counter]);
			counter++;

			std::cin.get();
		}
	}
	void physicalAttack(Enemy& enemy) {
		int playerPower = _stats.baseStats[(size_t)CombatStat::POWER]; //Player power without equipment or buffs.
		//Loop through equipped items and add modifiers.
		for (auto& e : _equipped) {
			if (e) {
				playerPower += e->getModifiers().flatModifiers[(size_t)CombatStat::POWER];
				playerPower = static_cast<int>(playerPower * e->getModifiers().multipliers[(size_t)CombatStat::POWER]);//This works only if multiplier isn't 0, otherwise things go to shit.
			}
		}
		int enemyDefense = enemy.GetStats().baseStats[(size_t)CombatStat::FORTITUDE];
		for (auto& e : enemy.GetEquipped()) {
			enemyDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::FORTITUDE];
			enemyDefense = static_cast<int>(enemyDefense * e->getModifiers().multipliers[(size_t)CombatStat::FORTITUDE]);//Again, only works if multiplier is not 0.
		}

		//Calculate damage based on some formula, IDK
		int damage = enemyDefense - playerPower;
		if (damage < 0)
			damage = 0;//Make sure we're not healing the enemy.
		enemy.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("You attacked, doing {} points of damage!", damage);
	}

	void magicAttack(Enemy& enemy) {
		int playerPower = _stats.baseStats[(size_t)CombatStat::SORCERY];
		for (auto& e : _equipped) {
			if (e) {
				playerPower += e->getModifiers().flatModifiers[(size_t)CombatStat::SORCERY];
				playerPower = static_cast<int>(playerPower * e->getModifiers().multipliers[(size_t)CombatStat::SORCERY]);
			}
		}
		int enemyDefense = enemy.GetStats().baseStats[(size_t)CombatStat::WILLPOWER];
		for (auto& e : _equipped) {
			if (e) {
				enemyDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::WILLPOWER];
				enemyDefense = static_cast<int>(enemyDefense * e->getModifiers().multipliers[(size_t)CombatStat::WILLPOWER]);
			}
		}

		int damage = playerPower - enemyDefense;
		if (damage < 0)
			damage = 0;
		enemy.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("You threw a fireball, doing {} points of damage!", damage);
	}
};