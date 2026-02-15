#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Entity.h"
#include "Components.h"
#include "MathUtils.h"
#include "EncounterSystem.h"
#include "PlayerMovementSystem.h"

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

    bool running = true;

    while (running)
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
                running = false;
                break;
            }
        }
        if (!running) break;

        std::cout << "\nMove (W/A/S/D) or Q to quit: ";
        char input;
        std::cin >> input;
        if (input == 'q') running = false;

        //running = PlayerMovementSystem::HandleInputMove(playerPos, input, 1.0f);
        PlayerMovementSystem::HandleInputMove(playerPos, input, 1.0f);
    }

    return 0;
}
