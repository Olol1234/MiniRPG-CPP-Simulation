#include "CombatRules.h"
#include "CombatSystem.h"
#include "CombatStatsComponent.h"
#include "Definitions.h"
#include "CombatTurn.h"
#include "CombatResult.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

CombatSystem::CombatSystem()
{
	font.loadFromFile("C:/Windows/Fonts/arial.ttf");

	attackButton.setSize({ 200.f, 50.f });
	attackButton.setFillColor(sf::Color(80, 80, 80));
	attackButton.setPosition(150.f, 480.f);

	attackText.setFont(font);
	attackText.setString("Attack");
	attackText.setCharacterSize(20);
	attackText.setFillColor(sf::Color::White);
	attackText.setPosition(200.f, 490.f);
}

//CombatResult CombatSystem::StartCombat(
//	CombatStatsComponent& playerStats,
//	const CharacterDefinition* enemyDef
//)
//{
//	CombatStatsComponent enemyStats;
//	enemyStats.characterDefinition = enemyDef;
//	enemyStats.currentHP = enemyDef->baseMaxHP;
//
//	int& playerHP = playerStats.currentHP;
//	int enemyHP = enemyStats.currentHP;
//
//	int playerAttack = playerStats.characterDefinition->baseAttack;
//	int enemyAttack = enemyDef->baseAttack;
//
//	int playerDefense = playerStats.characterDefinition->baseDefense;
//	int enemyDefense = enemyDef->baseDefense;
//
//	CombatTurn currentTurn = CombatTurn::Player;
//
//	std::cout << "\n=== COMBAT START ===\n";
//
//	while (playerHP > 0 && enemyHP > 0)
//	{
//		StartTurn(currentTurn);
//
//		CombatAction action;
//
//		if (currentTurn == CombatTurn::Player)
//		{
//			action = ChoosePlayerAction();
//			ExecuteAction(
//				action,
//				playerHP,
//				enemyHP,
//				playerAttack,
//				enemyDefense,
//				true
//			);
//			std::cout << "Enemy HP left " << enemyHP << "\n";
//		}
//		else
//		{
//			action = ChooseEnemyAction();
//
//			ExecuteAction(
//				action,
//				enemyHP,
//				playerHP,
//				enemyAttack,
//				playerDefense,
//				false
//			);
//			std::cout << "Player HP left " << playerHP << "\n";
//		}
//		EndTurn(currentTurn);
//
//		// Switch Turn
//		if (currentTurn == CombatTurn::Player)
//			currentTurn = CombatTurn::Enemy;
//		else
//			currentTurn = CombatTurn::Player;
//	}
//
//	if (playerHP > 0)
//	{
//		std::cout << "Player wins!\n";
//		return CombatResult::PlayerWin;
//	}
//	else
//	{
//		std::cout << "Player loses!\n";
//		return CombatResult::PlayerLose;
//	}
//
//	std::cout << "=== COMBAT END ===\n";
//}

//void CombatSystem::StartTurn(CombatTurn turn)
//{
//	if (turn == CombatTurn::Player)
//		std::cout << "Player prepares action...\n";
//	else
//		std::cout << "Enemy prepares action...\n";
//}
//
//void CombatSystem::EndTurn(CombatTurn turn)
//{
//	if (turn == CombatTurn::Player)
//		std::cout << "Player turn ends.\n";
//	else
//		std::cout << "Enemy turn ends.\n";
//}

//CombatAction CombatSystem::ChoosePlayerAction()
//{
//	std::cout << "\nChoose Action:\n";
//	std::cout << "1. Attack\n";
//	std::cout << "2. Defend (not implemented)\n";
//	std::cout << "3. Run (not implemented)\n";
//
//	int input;
//	std::cin >> input;
//
//	if (input == 1) return CombatAction::Attack;
//
//	return CombatAction::Attack; // fallback
//}

//CombatAction CombatSystem::ChooseEnemyAction()
//{
//	return CombatAction::Attack;
//}

//void CombatSystem::ExecuteAction(
//	CombatAction action,
//	int& attackerHP,
//	int& defenderHP,
//	int attackerAttack,
//	int defenderDefense,
//	bool isPlayerAttacker
//)
//{
//	if (action == CombatAction::Attack)
//	{
//		int dmg = CombatRules::CalculateDamage(attackerAttack, defenderDefense);
//		defenderHP -= dmg;
//
//		if (isPlayerAttacker)
//		{
//			std::cout << "Player attacks for " << dmg << "\n";
//		}
//		else
//		{
//			std::cout << "Enemy attacks for " << dmg << "\n";
//		}
//	}
//}

void CombatSystem::BeginCombat(
	CombatStatsComponent& player,
	WorldEnemy& enemyWorld
)
{
	combatPhase = CombatPhase::PlayerChoosing;

	playerStats = &player;
	currentEnemy = &enemyWorld;

	playerHP = player.currentHP;
	enemyHP = currentEnemy->enemyDef->baseMaxHP;

	currentTurn = CombatTurn::Player;
	combatFinished = false;
}

void CombatSystem::Update(float dt)
{
	if (combatFinished) return;

	actionTimer += dt;

	if (actionTimer < actionDelay) return;

	//if (currentTurn == CombatTurn::Player)
	if (combatPhase == CombatPhase::PlayerChoosing)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			ExecutePlayerAttack();
		}
	}
	else if (combatPhase == CombatPhase::EnemyActing)
	{
		actionTimer = 0.0f;
		int dmg = CombatRules::CalculateDamage(
			currentEnemy->enemyDef->baseAttack,
			playerStats->characterDefinition->baseDefense
		);
		playerHP -= dmg;
		if (playerHP < 0) playerHP = 0;
		combatPhase = CombatPhase::PlayerChoosing;
	}

	if (playerHP <= 0 || enemyHP <= 0)
	{
		combatFinished = true;

		if (playerHP > 0)
		{
			result = CombatResult::PlayerWin;
			playerStats->currentHP = playerHP;
		}
		else
		{
			result = CombatResult::PlayerLose;
			playerStats->currentHP = 0;
		}
	}
}

void CombatSystem::HandleMouseClick(sf::Vector2f mousePos)
{
	//if (combatPhase != CombatPhase::PlayerChoosing)
	//	return;

	if (attackButton.getGlobalBounds().contains(mousePos))
	{
		ExecutePlayerAttack();
	}
}

void CombatSystem::ExecutePlayerAttack()
{
	if (combatPhase != CombatPhase::PlayerChoosing)
		return;

	actionTimer = 0.0f;

	int dmg = CombatRules::CalculateDamage(
		playerStats->characterDefinition->baseAttack,
		currentEnemy->enemyDef->baseDefense
	);

	enemyHP -= dmg;
	if (enemyHP < 0) enemyHP = 0;
	combatPhase = CombatPhase::EnemyActing;
}

void CombatSystem::Render(sf::RenderWindow& window)
{
	sf::RectangleShape playerShape({ 80.f, 80.f });
	playerShape.setFillColor(sf::Color::Blue);
	playerShape.setPosition(100.f, 350.f);

	sf::RectangleShape enemyShape({ 80.f, 80.f });
	enemyShape.setFillColor(sf::Color::Red);
	enemyShape.setPosition(500.f, 150.f);

	window.draw(playerShape);
	window.draw(enemyShape);

	// === HP Bar ===
	float playerHPPercent = (float)playerHP /
		playerStats->characterDefinition->baseMaxHP;

	sf::RectangleShape playerHPBarBack({ 200.f, 20.f });
	playerHPBarBack.setFillColor(sf::Color(100, 100, 100));
	playerHPBarBack.setPosition(80.f, 320.f);

	sf::RectangleShape playerHPBarFront({ 200.f * playerHPPercent, 20.f });
	playerHPBarFront.setFillColor(sf::Color::Green);
	playerHPBarFront.setPosition(80.f, 320.f);

	window.draw(playerHPBarBack);
	window.draw(playerHPBarFront);

	float enemyHPPercent = (float)enemyHP / currentEnemy->enemyDef->baseMaxHP;

	sf::RectangleShape enemyHPBarBack({ 200.f, 20.f });
	enemyHPBarBack.setFillColor(sf::Color(100, 100, 100));
	enemyHPBarBack.setPosition(480.f, 120.f);

	sf::RectangleShape enemyHPBarFront({ 200.f * enemyHPPercent, 20.f });
	enemyHPBarFront.setFillColor(sf::Color::Green);
	enemyHPBarFront.setPosition(480.f, 120.f);

	window.draw(enemyHPBarBack);
	window.draw(enemyHPBarFront);

	if (combatPhase == CombatPhase::PlayerChoosing)
	{
		sf::RectangleShape menuBox({ 600.f, 120.f });
		menuBox.setFillColor(sf::Color(50, 50, 50));
		menuBox.setPosition(100.f, 450.f);

		//sf::Text attackText("1. Attack", font, 20);
		//attackText.setPosition(150.f, 480.f);
		//attackText.setFillColor(sf::Color::Blue);

		sf::Text defendText("2. Defend", font, 20);
		defendText.setPosition(350.f, 480.f);
		defendText.setFillColor(sf::Color::Blue);

		sf::Text runText("3. Run", font, 20);
		runText.setPosition(550.f, 480.f);
		runText.setFillColor(sf::Color::Blue);

		window.draw(menuBox);
		window.draw(attackButton);
		window.draw(attackText);
		window.draw(defendText);
		window.draw(runText);
		//window.draw(menuBox);
	}
}

bool CombatSystem::IsFinished() const
{
	return combatFinished;
}

CombatResult CombatSystem::GetResult() const
{
	return result;
}
