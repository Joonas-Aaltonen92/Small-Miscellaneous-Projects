#pragma once

struct Vec2 {
	int x{};
	int y{};

	Vec2 operator+(const Vec2& other)const {
		return { x + other.x, y + other.y };
	};
	Vec2& operator = (const Vec2& other) {
		x = other.x;
		y = other.y;
		return *this;
	};

	bool operator==(const Vec2& other)const {
		return x == other.x && y == other.y;
	};

	bool operator!=(const Vec2& other)const {
		return !(*this == other);
	}
};