#include "CombatRules.h"

int CombatRules::CalculateDamage(int attack, int defense)
{
	int dmg = attack - defense;
	if (dmg < 1) dmg = 1;
	return dmg;
}