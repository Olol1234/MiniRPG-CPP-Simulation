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
	//sf::RectangleShape playerShape({ 80.f, 80.f });
	//playerShape.setFillColor(sf::Color::Blue);
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerStats->characterDefinition->texture);
	playerSprite.setPosition(50.f, 200.f);
	playerSprite.setScale(0.5f, 0.5f);

	//sf::RectangleShape enemyShape({ 80.f, 80.f });
	//enemyShape.setFillColor(sf::Color::Red);
	sf::Sprite enemySprite;
	enemySprite.setTexture(currentEnemy->enemyDef->texture);
	enemySprite.setPosition(520.f, 80.f);
	enemySprite.setScale(0.5f, 0.5f);

	window.draw(playerSprite);
	window.draw(enemySprite);

	// === HP Bar ===
	float playerHPPercent = (float)playerHP /
		playerStats->characterDefinition->baseMaxHP;

	sf::RectangleShape playerHPBarBack({ 200.f, 20.f });
	playerHPBarBack.setFillColor(sf::Color(100, 100, 100));
	playerHPBarBack.setPosition(70.f, 200.f);

	sf::RectangleShape playerHPBarFront({ 200.f * playerHPPercent, 20.f });
	playerHPBarFront.setFillColor(sf::Color::Green);
	playerHPBarFront.setPosition(70.f, 200.f);

	window.draw(playerHPBarBack);
	window.draw(playerHPBarFront);

	float enemyHPPercent = (float)enemyHP / currentEnemy->enemyDef->baseMaxHP;

	sf::RectangleShape enemyHPBarBack({ 200.f, 20.f });
	enemyHPBarBack.setFillColor(sf::Color(100, 100, 100));
	enemyHPBarBack.setPosition(540.f, 80.f);

	sf::RectangleShape enemyHPBarFront({ 200.f * enemyHPPercent, 20.f });
	enemyHPBarFront.setFillColor(sf::Color::Green);
	enemyHPBarFront.setPosition(540.f, 80.f);

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
