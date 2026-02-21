#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Components.h"
#include "MathUtils.h"
#include "EncounterSystem.h"
#include "PlayerMovementSystem.h"
#include "GameState.h"
#include "CombatSystem.h"
#include "DefinitionManager.h"
#include "CombatStatsComponent.h"
#include "CombatResult.h"

int main()
{
    std::srand((unsigned)time(nullptr));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mini RPG");

    // ===== PLAYER =====
    Entity player = 1;
    PositionComponent playerPos{ 0.0f, 0.0f };

    // Loader
    DefinitionManager defManager;
    defManager.LoadCharacterDef("player", "data/player.json");
    defManager.LoadCharacterDef("enemy_goblin", "data/enemy_goblin.json");

    // === PLAYER RUNTIME STAT ===
    const CharacterDefinition* playerDefinition = defManager.GetCharacterDef("player");
    if (!playerDefinition)
    {
        std::cout << "Failed to get playerDefinition.\n";
        return 1;
    }
    CombatStatsComponent playerStats;
    playerStats.characterDefinition = playerDefinition;
    playerStats.currentHP = playerDefinition->baseMaxHP;

    // ===== SPAWN ENEMY =====
    const CharacterDefinition* enemyGoblinDef = defManager.GetCharacterDef("enemy_goblin");
    if (!enemyGoblinDef)
    {
        std::cout << "Failed to get enemy definition\n";
        return 1;
    }
    //std::vector<WorldEnemy> enemies;
	std::vector<std::unique_ptr<WorldEnemy>> enemies;
    enemies.reserve(5);
    for (size_t i = 0; i < 5; i++)
    {
        PositionComponent enemyPos;
        float minSpawnDistance = 5.0f;
        do
        {
            enemyPos.x = (float)(rand() % 20 - 10);
            enemyPos.y = (float)(rand() % 20 - 10);

        } while (Distance( playerPos.x, playerPos.y,
            enemyPos.x, enemyPos.y) < minSpawnDistance);
        //WorldEnemy enemy;
        //enemy.position = enemyPos;
        //enemy.enemyDef = enemyGoblinDef;
        //enemies.push_back(enemy);
        auto enemy = std::make_unique<WorldEnemy>();
        enemy->position = enemyPos;
        enemy->enemyDef = enemyGoblinDef;
        enemies.push_back(std::move(enemy));
    }

    // ===== DRAW SHAPE FOR ENEMY AND PLAYER =====
    sf::Sprite playerSprite;
    sf::Sprite enemySprite;

    playerSprite.setTexture(playerDefinition->texture);
    playerSprite.setScale(0.07f, 0.07f);
    enemySprite.setTexture(enemyGoblinDef->texture);
    enemySprite.setScale(0.07f, 0.07f);
    playerSprite.setOrigin(
        playerSprite.getLocalBounds().width / 2.f,
        playerSprite.getLocalBounds().height / 2.f
    );
    enemySprite.setOrigin(
        enemySprite.getLocalBounds().width / 2.f,
        enemySprite.getLocalBounds().height / 2.f
    );

    GameState gameState = GameState::Exploration;
    int currentEnemyIndex = -1;
    sf::Clock deltaClock;
    CombatSystem combatSystem;

    // View
    //sf::View explorationView(sf::FloatRect(0, 0, 400, 300));
    sf::View explorationView(sf::FloatRect(0, 0, 800, 600));
    sf::View combatView(sf::FloatRect(0, 0, 800, 600));

    while (window.isOpen())
    {
        // Get the time elapsed since the last frame
        sf::Time dt = deltaClock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        switch (gameState)
        {
            case GameState::Exploration:
            {
                //explorationView.setCenter(playerPos.x * 20,
                //    playerPos.y * 20);
                explorationView.setCenter(0, 0);
                window.setView(explorationView);

                // 1. Check for Encounters
                for (int i = 0; i < enemies.size(); i++)
                {
                    //if (EncounterSystem::CheckEncounter(playerPos, enemies[i].position, 1.5f))
                    if (EncounterSystem::CheckEncounter(playerPos, enemies[i]->position, 1.5f))
                    {
                        currentEnemyIndex = i;
                        //combatSystem.BeginCombat(playerStats, enemies[currentEnemyIndex]);
                        combatSystem.BeginCombat(playerStats, *enemies[currentEnemyIndex]);
                        gameState = GameState::Combat;
                        break;
                    }
                }

                if (gameState == GameState::Combat) break;

                PlayerMovementSystem::HandleRealtimeInput(playerPos, 50.0f, deltaTime);
                break;
            }

            case GameState::Combat:
            {
                combatSystem.Update(deltaTime);
                window.setView(combatView);

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f mousePos =
                            window.mapPixelToCoords(
                                { event.mouseButton.x, event.mouseButton.y }
                            );

                        combatSystem.HandleMouseClick(mousePos);
                    }
                }

                if (combatSystem.IsFinished())
                {
                    CombatResult result = combatSystem.GetResult();

                    if (result == CombatResult::PlayerWin)
                    {
                        enemies.erase(enemies.begin() + currentEnemyIndex);
                        gameState = GameState::Exploration;
                    }
                    else
                    {
                        gameState = GameState::Exit;
                    }
                }
            }
        }
        // Render
        //window.clear(sf::Color::Green);
        window.clear(
            gameState == GameState::Exploration
            ? sf::Color::Green
            : sf::Color::Cyan
        );

        if (gameState == GameState::Exploration)
        {
            // Draw
            float centerX = 400; // half of 800
            float centerY = 300; // half of 600

            float playerIsoX = (playerPos.x - playerPos.y) * 32.0f;
            float playerIsoY = (playerPos.x + playerPos.y) * 16.0f;

            //playerShape.setPosition(centerX + playerIsoX, centerY + playerIsoY);
            //playerSprite.setPosition(centerX + playerPos.x * 20,
            //    centerY + playerPos.y * 20);
            playerSprite.setPosition(playerPos.x * 20, playerPos.y * 20);
            window.draw(playerSprite);

            for (const auto& enemy : enemies)
            {
                // For isometric rendering
                //float isoX = (enemy.position.x - enemy.position.y) * 32.0f;
                //float isoY = (enemy.position.x + enemy.position.y) * 16.0f;

                //enemySprite.setPosition(centerX + enemy.position.x * 20,
                //    centerY + enemy.position.y * 20);
                //enemyShape.setPosition(centerX + isoX, centerY + isoY);
                //enemySprite.setPosition(enemy.position.x * 20, enemy.position.y * 20);
                enemySprite.setPosition(enemy->position.x * 20, enemy->position.y * 20);
                window.draw(enemySprite);
            }
        }
        else if (gameState == GameState::Combat)
        {
            combatSystem.Render(window);
        }
        window.display();
    }

    if (gameState == GameState::Exit) return 0;
    return 0;
}
