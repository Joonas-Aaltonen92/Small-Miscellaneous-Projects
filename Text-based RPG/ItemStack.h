#pragma once
#include <memory>
#include "Item.h"

struct ItemStack {
	std::shared_ptr<Item> _item;
	int _quantity = 1;

	ItemStack(std::shared_ptr<Item> i, int qt = 1) : _item(std::move(i)), _quantity(qt) {}

	void add(int amount = 1) {
		int maxStack = _item->maxStackSize();
		_quantity += amount;
		if (_quantity > maxStack)
			_quantity = maxStack;
	}
	void remove(int amount = 1) {
		_quantity -= amount;
		if(_quantity < 0)
			_quantity = 0;
	}
	bool isEmpty() const {
		return _quantity == 0;
	}
};