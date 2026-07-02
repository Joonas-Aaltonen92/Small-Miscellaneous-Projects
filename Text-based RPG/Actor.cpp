#include "Actor.h"

bool Actor::equals(const Actor& other) const {
	const auto& a = static_cast<const Actor&>(other);
	return _id == a._id && _name == a._name && _description == a._description;
}

bool Actor::operator==(const Actor& other) const {
	return typeid(*this) == typeid(other) && equals(other);
}
bool Actor::operator!=(const Actor& other) const {
	return !(*this == other);
}

bool NPC::equals(const Actor& other) const {
	if (!Actor::equals(other)) return false;
	const auto& n = static_cast<const NPC&>(other);
	return _dialogue == n._dialogue;
}