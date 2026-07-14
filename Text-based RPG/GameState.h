#pragma once
#include <string>
#include <unordered_map>
#include "PlayerState.h"
#include "RoomState.h"

struct GameState {
	PlayerState player;

	std::unordered_map<std::string, RoomState> rooms;
};