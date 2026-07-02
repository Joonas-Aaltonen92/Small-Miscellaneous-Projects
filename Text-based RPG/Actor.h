#pragma once
#include <string>
#include <memory>
#include <vector>

class Actor{
protected:
	std::string _id;
	std::string _name;
	std::string _description;

	virtual bool equals(const Actor& other) const;

public:
	Actor(std::string id, const std::string& name, const std::string& description) : _id(id), _name(name), _description(description) {}
	virtual ~Actor() = default;
	Actor(const Actor&) = default;
	Actor(Actor&&) = default;
	Actor& operator=(const Actor&) = default;
	Actor& operator=(Actor&&) = default;
	bool operator==(const Actor& other) const;
	bool operator!=(const Actor& other) const;

	const std::string& getID() const { return _id; }
	const std::string& getName() const { return _name; }

	virtual std::unique_ptr<Actor> clone() const = 0;
};

class NPC : public Actor {
protected:
	std::vector<std::string> _dialogue;
	
	bool equals(const Actor& other) const override;
public:
	NPC(std::string id, const std::string& name, const std::string& description, std::vector<std::string> dialogue) : Actor(id, name, description), _dialogue(std::move(dialogue)) {}
	std::unique_ptr<Actor> clone() const override {
		return std::make_unique<NPC>(*this);
	}
	const std::vector<std::string>& getDialogue() const { return _dialogue; }
};