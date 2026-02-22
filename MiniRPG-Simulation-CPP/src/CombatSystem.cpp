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
	WorldEnemy& enemyWorld,
	bool isPlayerWet,
	bool isEnemyWet
)
{
	this->playerWet = isPlayerWet;
	this->enemyWet = isEnemyWet;

	combatPhase = CombatPhase::PlayerChoosing;

	playerStats = &player;
	currentEnemy = &enemyWorld;

	playerHP = player.currentHP;
	enemyHP = currentEnemy->enemyDef->baseMaxHP;

	currentTurn = CombatTurn::Player;
	combatFinished = false;

	auto& attacks = playerStats->characterDefinition->attacks;
	for (size_t i = 0; i < attacks.size(); ++i)
	{
		//skillButtons.clear();
		//skillButtonTexts.clear();

		sf::RectangleShape button({ 180.f, 50.f });
		button.setFillColor(sf::Color(80, 80, 80));
		button.setPosition(120.f + i * 190.f, 500.f);
		skillButtons.push_back(button);

		sf::Text txt(std::to_string(i+1) + ". " + attacks[i].name, font, 20);
		txt.setFillColor(sf::Color::White);
		txt.setPosition(130.f + i * 190.f, 510.f);
		skillButtonTexts.push_back(txt);
	}
}

void CombatSystem::Update(float dt)
{
	if (combatFinished) return;

	actionTimer += dt;

	if (actionTimer < actionDelay) return;

	//if (currentTurn == CombatTurn::Player)
	if (combatPhase == CombatPhase::PlayerChoosing)
	{
		if (combatPhase == CombatPhase::PlayerChoosing) {
			auto& attacks = playerStats->characterDefinition->attacks;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) ExecutePlayerAttack(0);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) 
				&& attacks.size() > 1) ExecutePlayerAttack(1);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) 
				&& attacks.size() > 2) ExecutePlayerAttack(2);
		}
	}
	else if (combatPhase == CombatPhase::EnemyActing)
	{
		if (enemyFrozenTurns > 0)
		{
			enemyFrozenTurns--;
			actionTimer = 0.0f;
			combatPhase = CombatPhase::PlayerChoosing;
		}
		else
		{
			actionTimer = 0.0f;
			auto& attacks = currentEnemy->enemyDef->attacks[1];
			int dmg = CombatRules::CalculateDamage(
				attacks.damage,
				playerStats->characterDefinition->baseDefense
			);
			playerHP -= dmg;
			if (playerHP < 0) playerHP = 0;
			combatPhase = CombatPhase::PlayerChoosing;
		}
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
	if (combatPhase != CombatPhase::PlayerChoosing) return;

	//if (attackButton.getGlobalBounds().contains(mousePos))
	//{
		//ExecutePlayerAttack();
	for (size_t i = 0; i < skillButtons.size(); ++i) {
		if (skillButtons[i].getGlobalBounds().contains(mousePos)) {
			ExecutePlayerAttack(i);
			break;
		}
	}
	//}
}

void CombatSystem::ExecutePlayerAttack(int attackIndex)
{
	if (combatPhase != CombatPhase::PlayerChoosing)
		return;

	actionTimer = 0.0f;

	auto& attacks = playerStats->characterDefinition->attacks[attackIndex];

	int dmg = CombatRules::CalculateDamage(
		attacks.damage, currentEnemy->enemyDef->baseDefense);
	// Check for wet status effects
	if (enemyWet && attacks.element == "electric")
	{
		dmg *= 2; // Double damage if enemy is wet and attack is electric
	}
	else if (enemyWet && attacks.element == "ice")
	{
		int chanceToFreeze = std::rand() % 100 + 1;
		if (chanceToFreeze <= 20)
		{
			enemyFrozenTurns = 2;
		}
	}

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

	// === Player HP Bar ===
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

	// === Enemy HP Bar ===
	float enemyHPPercent = (float)enemyHP / 
		currentEnemy->enemyDef->baseMaxHP;

	sf::RectangleShape enemyHPBarBack({ 200.f, 20.f });
	enemyHPBarBack.setFillColor(sf::Color(100, 100, 100));
	enemyHPBarBack.setPosition(540.f, 80.f);

	sf::RectangleShape enemyHPBarFront({ 200.f * enemyHPPercent, 20.f });
	enemyHPBarFront.setFillColor(sf::Color::Green);
	enemyHPBarFront.setPosition(540.f, 80.f);

	window.draw(enemyHPBarBack);
	window.draw(enemyHPBarFront);

	// === Enemy Status Effects ===
	if (enemyFrozenTurns > 0)
	{
		sf::RectangleShape frozenIcon({ 20.f, 20.f });
		frozenIcon.setFillColor(sf::Color::Blue);
		frozenIcon.setPosition(540.f, 110.f);
		window.draw(frozenIcon);
	}

	if (combatPhase == CombatPhase::PlayerChoosing)
	{
		sf::RectangleShape menuBox({ 700.f, 150.f });
		menuBox.setFillColor(sf::Color(50, 50, 50));
		menuBox.setPosition(50.f, 440.f);

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
		//window.draw(attackButton);
		//window.draw(attackText);
		//window.draw(defendText);
		//window.draw(runText);
		for (size_t i = 0; i < skillButtons.size(); ++i)
		{
			window.draw(skillButtons[i]);
			if (i < skillButtonTexts.size())
				window.draw(skillButtonTexts[i]);
		}
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
