#pragma once
#include <memory>
#include "Item.h"

struct ItemStack {
	std::shared_ptr<Item> _item;
	int _quantity = 1;

	ItemStack(std::shared_ptr<Item> i, int qt = 1) : _item(std::move(i)), _quantity(qt) {}

	void add(int amount = 1);
	void remove(int amount = 1);
	bool isEmpty() const;
};