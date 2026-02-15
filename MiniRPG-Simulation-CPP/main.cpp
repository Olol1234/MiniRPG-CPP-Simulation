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

int main()
{
    std::srand((unsigned)time(nullptr));
    std::cout << "Mini RPG Simulation Started" << std::endl;

    // ===== PLAYER =====
    Entity player = 1;
    PositionComponent playerPos{ 0.0f, 0.0f };

    // ===== ENEMY =====
    std::vector<PositionComponent> enemies;
    int enemyCount = 5;
    for (int i = 0; i < enemyCount; i++)
    {
        PositionComponent enemyPos
        {
            (float)(rand() % 20 - 10),
            (float)(rand() % 20 - 10)
        };
        enemies.push_back(enemyPos);
    }

    // ===== PRINT ENEMIES POS TO CHECK =====
    std::cout << "\nEnemies\n";
    for (int i = 0; i < enemies.size(); i++)
    {
        std::cout << "Enemy " << i
                << ": (" << enemies[i].x
                << ", " << enemies[i].y
                << ")\n";
    }

    //bool running = true;
    GameState gameState = GameState::Exploration;

    while (gameState != GameState::Exit)
    {
        std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
        for (int i = 0; i < enemies.size(); i++)
        {
            float dist = Distance(
                playerPos.x,
                playerPos.y,
                enemies[i].x,
                enemies[i].y
            );
            std::cout << "Distance to Enemy " << i << ": " << dist << "\n";
            if (EncounterSystem::CheckEncounter(playerPos, enemies[i], 1.5f))
            {
                std::cout << "\n ENCOUNTER WITH ENEMY " << i << "!\n";
                CombatSystem::StartCombat(playerPos, enemies[i]);
                gameState = GameState::Exit;
                break;
            }
        }
        // INPUT
        std::cout << "\nMove (W/A/S/D) or Q to quit: ";
        char input;
        std::cin >> input;
        if (input == 'q')
        {
            gameState = GameState::Exit;
            break;
        }
        PlayerMovementSystem::HandleInputMove(playerPos, input, 1.0f);
    }
    if (gameState == GameState::Exit) return 0;
    return 0;
}
