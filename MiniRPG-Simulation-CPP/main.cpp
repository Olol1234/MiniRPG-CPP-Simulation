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
    //std::cout << "Mini RPG Simulation Started" << std::endl;
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
    std::vector<WorldEnemy> enemies;
    enemies.reserve(5);
    for (size_t i = 0; i < 5; i++)
    {
        PositionComponent enemyPos
        {
            (float)(rand() % 20 - 10),
            (float)(rand() % 20 - 10)
        };
        WorldEnemy enemy;
        enemy.position = enemyPos;
        enemy.enemyDef = enemyGoblinDef;
        enemies.push_back(enemy);
    }

    // ===== PRINT ENEMIES POS TO CHECK =====
    std::cout << "\nEnemies\n";
    for (int i = 0; i < enemies.size(); i++)
    {
        std::cout << "Enemy " << i
                << ": (" << enemies[i].position.x
                << ", " << enemies[i].position.y
                << ")\n";
    }

    // ===== DRAW SHAPE FOR ENEMY AND PLAYER =====
    sf::RectangleShape playerShape(sf::Vector2f(20.f, 20.f));
    playerShape.setFillColor(sf::Color::Blue);

    sf::RectangleShape enemyShape(sf::Vector2f(20.f, 20.f));
    enemyShape.setFillColor(sf::Color::Red);

    //bool running = true;
    GameState gameState = GameState::Exploration;
    int currentEnemyIndex = -1;
    sf::Clock deltaClock;

    //while (gameState != GameState::Exit)
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
                //std::cout << "\n--- Exploration Mode ---";
                //std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
                //// Print enemy pos again
                //std::cout << "\nEnemies\n";
                //for (int i = 0; i < enemies.size(); i++)
                //{
                //    std::cout << "Enemy " << i
                //        << ": (" << enemies[i].position.x
                //        << ", " << enemies[i].position.y
                //        << ")\n";
                //}

                // 1. Check for Encounters
                for (int i = 0; i < enemies.size(); i++)
                {
                    if (EncounterSystem::CheckEncounter(playerPos, enemies[i].position, 1.5f))
                    {
                        //std::cout << "\n ENCOUNTER WITH ENEMY " << i << "!\n";
                        currentEnemyIndex = i;
                        gameState = GameState::Combat;
                        break;
                    }
                }

                if (gameState == GameState::Combat) break; // Exit exploration logic if combat started

                // 2. Handle Movement
                //std::cout << "Move (W/A/S/D) or Q to quit: ";
                //char input;
                //std::cin >> input;
                //if (input == 'q') { gameState = GameState::Exit; break; }

                PlayerMovementSystem::HandleRealtimeInput(playerPos, 50.0f, deltaTime);
                break;
            }

            case GameState::Combat:
            {
                //std::cout << "\n--- Combat Mode ---";

                CombatResult result = CombatSystem::StartCombat(playerStats, enemies[currentEnemyIndex].enemyDef);
                if (result == CombatResult::PlayerWin)
                {
                    std::cout << "Enemy defeated!\n";
                    enemies.erase(enemies.begin() + currentEnemyIndex);
                    gameState = GameState::Exploration;
                }
                else
                {
                    std::cout << "Player died. Game Over.\n";
                    gameState = GameState::Exit;
                }
                break;
            }
        }
        // Render
        window.clear(sf::Color::Green);
        // Draw
        float centerX = 400; // half of 800
        float centerY = 300; // half of 600

        //sf::RectangleShape playerShape(sf::Vector2f(20.f, 20.f));
        //playerShape.setFillColor(sf::Color::Blue);
        playerShape.setPosition(centerX + playerPos.x * 20,
            centerY + playerPos.y * 20);
        window.draw(playerShape);

        for (const auto& enemy : enemies)
        {
            //sf::RectangleShape enemyShape(sf::Vector2f(20.f, 20.f));
            //enemyShape.setFillColor(sf::Color::Red);
            enemyShape.setPosition(centerX + enemy.position.x * 20,
                centerY + enemy.position.y * 20);
            window.draw(enemyShape);
        }


        window.display();
    }

    if (gameState == GameState::Exit) return 0;
    return 0;
}
