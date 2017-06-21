#include "Character.h"

Character::Character()
{
	isAuto = true;
	maxHealth = health = 100;
}

Character::~Character()
{
}

void Character::RemoveHealth(float amountToRemove)
{
	health -= amountToRemove;
	healthBar->Update(health);
}
