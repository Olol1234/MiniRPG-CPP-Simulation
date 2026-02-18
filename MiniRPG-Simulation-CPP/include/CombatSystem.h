#pragma once

#include "Components.h"
#include "Definitions.h"
#include "CombatStatsComponent.h"
#include "CombatResult.h"
#include "CombatTurn.h"
#include "CombatAction.h"

class CombatSystem
{
public:
	static CombatResult StartCombat(
		CombatStatsComponent& playerStats,
		const CharacterDefinition* enemyDef
	);

private:
	static void StartTurn(CombatTurn turn);
	static void EndTurn(CombatTurn turn);

	static CombatAction ChoosePlayerAction();
	static CombatAction ChooseEnemyAction();
	static void ExecuteAction(
		CombatAction action,
		int& attackerHp,
		int& defenderHp,
		int attackerAttack,
		int attackerDefense,
		bool isPlayerAttacker
	);
};