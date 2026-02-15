#include <cassert>
#include "CombatRules.h"

int main()
{
	// Normal case
	assert(CombatRules::CalculateDamage(10, 5) == 5);
	// Defense higher
	assert(CombatRules::CalculateDamage(5, 10) == 1);
	// Equal
	assert(CombatRules::CalculateDamage(10, 10) == 1);

	return 0;
}