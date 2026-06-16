#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <iostream>
#include "Stats.h"
#include "Inventory.h"

class Player;
class Enemy;
class Container;
class Equipment;

class Actor{
protected:
	std::string _type;
	std::string _name;
	std::string _description;

	virtual bool equals(const Actor& other) const {
		const auto& a = static_cast<const Actor&>(other);
		return _type == a._type && _name == a._name && _description == a._description;
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