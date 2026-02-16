#include "CombatRules.h"
#include "CombatSystem.h"
#include "CombatStatsComponent.h"
#include "Definitions.h"
#include <iostream>

void CombatSystem::StartCombat(
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

	std::cout << "\n=== COMBAT START ===\n";
	while (playerHP > 0 && enemyHP > 0)
	{
		int dmgToEnemy = CombatRules::CalculateDamage(playerAttack, enemyDefense);
		enemyHP -= dmgToEnemy;
		std::cout << "Player deals " << dmgToEnemy << " damage. Enemy HP: " << enemyHP << "\n";
		if (enemyHP <= 0) break;

		int dmgToPlayer = CombatRules::CalculateDamage(enemyAttack, playerDefense);
		playerHP -= dmgToPlayer;
		std::cout << "Enemy deals " << dmgToPlayer << " damage. Player HP: " << playerHP << "\n";
	}
	if (playerHP > 0)
		std::cout << "Player wins!\n";
	else
		std::cout << "Player loses!\n";

	std::cout << "=== COMBAT END ===\n";
}