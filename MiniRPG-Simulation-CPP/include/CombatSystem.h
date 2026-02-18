#pragma once

#include "Components.h"
#include "Definitions.h"
#include "CombatStatsComponent.h"
#include "CombatResult.h"

class CombatSystem
{
public:
	static CombatResult StartCombat(
		CombatStatsComponent& playerStats,
		const CharacterDefinition* enemyDef
	);
};