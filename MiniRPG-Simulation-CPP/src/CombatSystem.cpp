#include "CombatRules.h"
#include "CombatSystem.h"
#include "CombatStatsComponent.h"
#include "Definitions.h"
#include "CombatTurn.h"
#include "CombatResult.h"
#include <iostream>

CombatResult CombatSystem::StartCombat(
	CombatStatsComponent& playerStats,
	const CharacterDefinition* enemyDef
)
{
	CombatStatsComponent enemyStats;
	enemyStats.characterDefinition = enemyDef;
	enemyStats.currentHP = enemyDef->baseMaxHP;

	int& playerHP = playerStats.currentHP;
	int enemyHP = enemyStats.currentHP;

	int playerAttack = playerStats.characterDefinition->baseAttack;
	int enemyAttack = enemyDef->baseAttack;

	int playerDefense = playerStats.characterDefinition->baseDefense;
	int enemyDefense = enemyDef->baseDefense;

	CombatTurn currentTurn = CombatTurn::Player;

	std::cout << "\n=== COMBAT START ===\n";

	while (playerHP > 0 && enemyHP > 0)
	{
		StartTurn(currentTurn);

		CombatAction action;

		if (currentTurn == CombatTurn::Player)
		{
			action = ChoosePlayerAction();
			ExecuteAction(
				action,
				playerHP,
				enemyHP,
				playerAttack,
				enemyDefense,
				true
			);
			std::cout << "Enemy HP left " << enemyHP << "\n";
		}
		else
		{
			action = ChooseEnemyAction();

			ExecuteAction(
				action,
				enemyHP,
				playerHP,
				enemyAttack,
				playerDefense,
				false
			);
			std::cout << "Player HP left " << playerHP << "\n";
		}
		EndTurn(currentTurn);

		// Switch Turn
		if (currentTurn == CombatTurn::Player)
			currentTurn = CombatTurn::Enemy;
		else
			currentTurn = CombatTurn::Player;
	}

	if (playerHP > 0)
	{
		std::cout << "Player wins!\n";
		return CombatResult::PlayerWin;
	}
	else
	{
		std::cout << "Player loses!\n";
		return CombatResult::PlayerLose;
	}

	std::cout << "=== COMBAT END ===\n";

}

void CombatSystem::StartTurn(CombatTurn turn)
{
	if (turn == CombatTurn::Player)
		std::cout << "Player prepares action...\n";
	else
		std::cout << "Enemy prepares action...\n";
}

void CombatSystem::EndTurn(CombatTurn turn)
{
	if (turn == CombatTurn::Player)
		std::cout << "Player turn ends.\n";
	else
		std::cout << "Enemy turn ends.\n";
}

CombatAction CombatSystem::ChoosePlayerAction()
{
	std::cout << "\nChoose Action:\n";
	std::cout << "1. Attack\n";
	std::cout << "2. Defend (not implemented)\n";
	std::cout << "3. Run (not implemented)\n";

	int input;
	std::cin >> input;

	if (input == 1) return CombatAction::Attack;

	return CombatAction::Attack; // fallback
}

CombatAction CombatSystem::ChooseEnemyAction()
{
	return CombatAction::Attack;
}

void CombatSystem::ExecuteAction(
	CombatAction action,
	int& attackerHP,
	int& defenderHP,
	int attackerAttack,
	int defenderDefense,
	bool isPlayerAttacker
)
{
	if (action == CombatAction::Attack)
	{
		int dmg = CombatRules::CalculateDamage(attackerAttack, defenderDefense);
		defenderHP -= dmg;

		if (isPlayerAttacker)
		{
			std::cout << "Player attacks for " << dmg << "\n";
		}
		else
		{
			std::cout << "Enemy attacks for " << dmg << "\n";
		}
	}
}
