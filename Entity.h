#pragma once

class Entity {
public:
	float x;
	float y;
	float z;

	float yaw;
	float pitch;

	bool team;

	int hp;
	char name[16];
	bool isGood;

	void print();
};

