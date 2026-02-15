#pragma once

#include "Components.h"

class CombatSystem
{
public:
	static void StartCombat(
		PositionComponent& playerPos,
		PositionComponent& enemyPos
	);
};