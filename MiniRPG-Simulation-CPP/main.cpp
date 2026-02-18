#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

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
    std::cout << "Mini RPG Simulation Started" << std::endl;

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

    //bool running = true;
    GameState gameState = GameState::Exploration;

    //while (gameState != GameState::Exit)
    //{
    //    std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
    //    for (int i = 0; i < enemies.size(); i++)
    //    {
    //        float dist = Distance(
    //            playerPos.x,
    //            playerPos.y,
    //            enemies[i].position.x,
    //            enemies[i].position.y
    //        );
    //        std::cout << "Distance to Enemy " << i << ": " << dist << "\n";
    //        if (EncounterSystem::CheckEncounter(playerPos, enemies[i].position, 1.5f))
    //        {
    //            std::cout << "\n ENCOUNTER WITH ENEMY " << i << "!\n";
    //            CombatResult result = CombatSystem::StartCombat(playerStats, enemies[i].enemyDef);
    //            if (result == CombatResult::PlayerWin)
    //            {
    //                std::cout << "Enemy defeated!\n";

    //                enemies.erase(enemies.begin() + i);
    //                i--;
    //                // Print enemy pos again
    //                std::cout << "\nEnemies\n";
    //                for (int i = 0; i < enemies.size(); i++)
    //                {
    //                    std::cout << "Enemy " << i
    //                        << ": (" << enemies[i].position.x
    //                        << ", " << enemies[i].position.y
    //                        << ")\n";
    //                }
    //            }
    //            else
    //            {
    //                std::cout << "Player died. Game Over.\n";
    //                gameState = GameState::Exit;
    //            }
    //            break;
    //        }
    //    }
    //    // INPUT
    //    std::cout << "\nMove (W/A/S/D) or Q to quit: ";
    //    char input;
    //    std::cin >> input;
    //    if (input == 'q')
    //    {
    //        gameState = GameState::Exit;
    //        break;
    //    }
    //    PlayerMovementSystem::HandleInputMove(playerPos, input, 1.0f);
    //    std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
    //}
    int currentEnemyIndex = -1;

    while (gameState != GameState::Exit)
    {
        switch (gameState)
        {
            case GameState::Exploration:
            {
                std::cout << "\n--- Exploration Mode ---";
                std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
                // Print enemy pos again
                std::cout << "\nEnemies\n";
                for (int i = 0; i < enemies.size(); i++)
                {
                    std::cout << "Enemy " << i
                        << ": (" << enemies[i].position.x
                        << ", " << enemies[i].position.y
                        << ")\n";
                }

                // 1. Check for Encounters
                for (int i = 0; i < enemies.size(); i++)
                {
                    if (EncounterSystem::CheckEncounter(playerPos, enemies[i].position, 1.5f))
                    {
                        std::cout << "\n ENCOUNTER WITH ENEMY " << i << "!\n";
                        currentEnemyIndex = i;
                        gameState = GameState::Combat;
                        break;
                    }
                }

                if (gameState == GameState::Combat) break; // Exit exploration logic if combat started

                // 2. Handle Movement
                std::cout << "Move (W/A/S/D) or Q to quit: ";
                char input;
                std::cin >> input;
                if (input == 'q') { gameState = GameState::Exit; break; }

                PlayerMovementSystem::HandleInputMove(playerPos, input, 1.0f);
                break;
            }

            case GameState::Combat:
            {
                std::cout << "\n--- Combat Mode ---";

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
    }

    if (gameState == GameState::Exit) return 0;
    return 0;
}
