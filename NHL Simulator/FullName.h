#pragma once
#include <string>

struct FullName {
public:
	std::string firstName;
	std::string lastName;

	// Return the full name; method is const so it does not modify the object.
	std::string fullName() const {
		return firstName + " " + lastName;
	}
};