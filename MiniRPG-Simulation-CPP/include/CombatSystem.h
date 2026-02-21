#pragma once

#include "Components.h"
#include "Definitions.h"
#include "CombatStatsComponent.h"
#include "CombatResult.h"
#include "CombatTurn.h"
#include "CombatAction.h"
#include <SFML/Graphics.hpp>

enum class CombatPhase
{
	PlayerChoosing,
	PlayerActing,
	EnemyActing,
	Finished
};

class CombatSystem
{
public:
	CombatSystem();
	//static CombatResult StartCombat(
	//	CombatStatsComponent& player,
	//	const CharacterDefinition* enemyDefinition
	//);
	void BeginCombat(
		CombatStatsComponent& playerStats,
		WorldEnemy& enemy
	);
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	bool IsFinished() const;
	CombatResult GetResult() const;
	void HandleMouseClick(sf::Vector2f mousePos);

private:
	//static void StartTurn(CombatTurn turn);
	//static void EndTurn(CombatTurn turn);

	//static CombatAction ChoosePlayerAction();
	//static CombatAction ChooseEnemyAction();
	//static void ExecuteAction(
	//	CombatAction action,
	//	int& attackerHp,
	//	int& defenderHp,
	//	int attackerAttack,
	//	int attackerDefense,
	//	bool isPlayerAttacker
	//);
	void ExecutePlayerAttack();

	CombatStatsComponent* playerStats;
	WorldEnemy* currentEnemy;
	int playerHP;
	int enemyHP;
	CombatTurn currentTurn;
	CombatResult result;
	bool combatFinished = false;

	CombatPhase combatPhase;
	sf::Font font;
	float actionTimer = 0.0f;
	float actionDelay = 0.6f;
	sf::RectangleShape attackButton;
	sf::Text attackText;
};