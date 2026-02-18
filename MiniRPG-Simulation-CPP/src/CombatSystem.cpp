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
		if (currentTurn == CombatTurn::Player)
		{
			std::cout << "\n--- Player Turn ---\n";

			int dmg = CombatRules::CalculateDamage(playerAttack, enemyDefense);
			enemyHP -= dmg;

			std::cout << "Player deals " << dmg
				<< " damage. Enemy HP: " << enemyHP << "\n";

			currentTurn = CombatTurn::Enemy;
		}
		else
		{
			std::cout << "\n--- Enemy Turn ---\n";

			int dmg = CombatRules::CalculateDamage(enemyAttack, playerDefense);
			playerHP -= dmg;

			std::cout << "Enemy deals " << dmg
				<< " damage. Player HP: " << playerHP << "\n";

			currentTurn = CombatTurn::Player;
		}
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

void StartTurn(CombatTurn turn)
{
	if (turn == CombatTurn::Player)
		std::cout << "Player prepares action...\n";
	else
		std::cout << "Enemy prepares action...\n";
}

void EndTurn(CombatTurn turn)
{
	if (turn == CombatTurn::Player)
		std::cout << "Player turn ends.\n";
	else
		std::cout << "Enemy turn ends.\n";
}
