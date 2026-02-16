#pragma once

#include "Components.h"
#include "Definitions.h"
#include "CombatStatsComponent.h"

class CombatSystem
{
public:
	static void StartCombat(
		CombatStatsComponent& playerStats,
		const CharacterDefinition* enemyDef
	);
};