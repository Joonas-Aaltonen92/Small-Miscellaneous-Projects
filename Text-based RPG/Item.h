#include <string>
#include <print>
#include <memory>
#include <typeinfo>
#include "Stats.h"

enum class ItemType : int {
	EQUIPMENT,
	CONSUMABLE,
	TRINKET,
	KEY,
	KEYITEM,
	UNKNOWN,
	COUNT
};

class Item {
protected:
	std::string _name;
	std::string _description;
	int _value;
	ItemType _type;

	virtual bool equals(const Item& other) const {
		const auto& o = static_cast<const Item&>(other);
		return ID == o.ID && _name == o._name && _description == o._description && _value == o._value && _type == o._type;
	}

public:
	const std::string ID;

	Item(std::string id, const std::string& name, const std::string& description, int value, ItemType type) : ID(std::move(id)), _name(name), _description(description), _value(value), _type(type) {}
	virtual ~Item() = default;
	
	Item(const Item&) = default;
	Item(Item&&) = default; 
	Item& operator=(const Item&) = default;
	Item& operator=(Item&&) = default;
	
	bool operator==(const Item& other) const {
		return typeid(*this) == typeid(other) && equals(other);
	}
	bool operator!=(const Item& other) const {
		return !(*this == other);
	}
	virtual std::unique_ptr<Item> clone() const = 0;

	void displayItem() const {
		std::println("Name: {}\nDescription: {}\nValue: {}\n", _name, _description, _value);
	}
	virtual bool isStackable() const { return false; }
	virtual int maxStackSize() const { return 1; }

	std::string getName() const { return _name; }
	std::string getDescription() const { return _description; }
	int getValue() const { return _value; }
	ItemType getType() const { return _type; }
};

enum class EquipmentSlot {
	HEAD,//Helmet, hat, cap...
	CHEST,//Chestplate, robe, shirt...
	WAIST,//Belts, etc...
	LEGS,//Pants, skirt...
	BODY,//Equipment that covers both the torso and legs, such as the infamous bikini armor
	FEET,//Boots, shoes...
	ARMS,//Gauntlets, bracers...
	HAND,//Weapon and shield, or both hands for two handed weapons/large shields
	RING,
	AMULET,
	NONE,
	COUNT
};

class Equipment : public Item{
private:
	EquipmentSlot _slot;
	StatModifiers _modifiers;

	bool equals(const Item& other) const override {
		if (!Item::equals(other)) return false;
		const auto& e = static_cast<const Equipment&>(other);
		return _slot == e._slot && _modifiers.flatModifiers == e._modifiers.flatModifiers && _modifiers.multipliers == e._modifiers.multipliers;
	}
public:
	Equipment(std::string id, const std::string& name, const std::string& description, int value, ItemType type = ItemType::EQUIPMENT, EquipmentSlot slot, StatModifiers modifiers) : Item(id, name, description, value, type), _slot(slot), _modifiers(modifiers) {}
	std::unique_ptr<Item> clone() const override {
		return std::make_unique<Equipment>(*this);
	}
	EquipmentSlot getSlot() const { return _slot; }
	StatModifiers getModifiers() const { return _modifiers; }
};

class Consumable : public Item {
	StatModifiers _modifiers;
	bool equals(const Item& other) const override {
		if (!Item::equals(other)) return false;
		const auto& c = static_cast<const Consumable&>(other);
		return _modifiers.flatModifiers == c._modifiers.flatModifiers && _modifiers.multipliers == c._modifiers.multipliers;
	}
public:
	Consumable(std::string id, const std::string& name, const std::string& description, int value, ItemType type = ItemType::CONSUMABLE, StatModifiers modifiers) : Item(id,name, description, value, type), _modifiers(modifiers) {}

	bool isStackable() const override { return true; }
	int maxStackSize() const override { return 99; }
	std::unique_ptr<Item> clone() const override {
		return std::make_unique<Consumable>(*this);
	}
};

//Trinkets are items that have no special properties other than being worth something. They're basically the base Item class and nothing else, but I want to be able to differentiate them from other items.
class Trinket : public Item {
public:
	Trinket(std::string id, const std::string& name, const std::string& description, int value, ItemType type = ItemType::TRINKET) : Item(id, name, description, value, type) {}
	bool isStackable() const override { return true; }
	int maxStackSize() const override { return 99; }
	std::unique_ptr<Item> clone() const override {
		return std::make_unique<Trinket>(*this);
	}
};
class Key : public Item {
public:
	Key(std::string id,const std::string& name, const std::string& description, int value, ItemType type = ItemType::KEY) : Item(id, name, description, value, type) {}
	bool isStackable() const override { return true; }
	int maxStackSize() const override { return 99; }
	std::unique_ptr<Item> clone() const override {
		return std::make_unique<Key>(*this);
	}
};
//Not to be confused with the Key class. KeyItems are items that are necessary for story progression, such as quest items.
class KeyItem : public Item {
public:
	KeyItem(std::string id, const std::string& name, const std::string& description, int value = 0, ItemType type = ItemType::KEYITEM) : Item(id, name, description, value, type) {}
	std::unique_ptr<Item> clone() const override {
		return std::make_unique<KeyItem>(*this);
	}
};