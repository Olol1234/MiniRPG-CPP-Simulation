#include "CombatRules.h"
#include "CombatSystem.h"
#include <iostream>

void CombatSystem::StartCombat(
	PositionComponent& playerPos,
	PositionComponent& enemyPos
)
{
	int playerHP = 20;
	int enemyHP = 10;

	int playerAttack = 8;
	int enemyAttack = 6;

	int playerDefense = 3;
	int enemyDefense = 2;

	std::cout << "\n=== COMBAT START ===\N";
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