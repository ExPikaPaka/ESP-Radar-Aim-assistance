#include "Entity.h"
#include <iostream>

void Entity::print() {
	if (isGood) {
		std::cout << "Name  : " << name << "\n" <<
					  "HP    : " << hp << "\n" <<
					  "X     : " << x << "\n" <<
					  "Y     : " << y << "\n" <<
					  "Z     : " << z << "\n" <<
					  "Team  : " << (team ? "Blue" : "Red") << "\n" <<
					  "Pitch : " << pitch << "\n" <<
					  "Yaw   : " << yaw << "\n";
	}
}