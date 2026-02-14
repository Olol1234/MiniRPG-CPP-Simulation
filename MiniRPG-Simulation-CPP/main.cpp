#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Entity.h"
#include "Components.h"
#include "MathUtils.h"
#include "EncounterSystem.h"

int main()
{
    std::srand((unsigned)time(nullptr));
    std::cout << "Mini RPG Simulation Started" << std::endl;

    // ===== PLAYER =====
    Entity player = 1;
    PositionComponent playerPos{ 0.0f, 0.0f };

    // ===== ENEMY =====
    Entity enemy = 2;
    PositionComponent enemyPos{
        (float)(rand() % 10 - 5),
        (float)(rand() % 10 - 5)
    };

    bool running = true;

    while (running)
    {
        std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
        std::cout << "Enemy Position: (" << enemyPos.x << ", " << enemyPos.y << ")\n";

        float dist = Distance(playerPos.x, playerPos.y, enemyPos.x, enemyPos.y);
        std::cout << "Distance to Enemy: " << dist << "\n";

        if (EncounterSystem::CheckEncounter(playerPos, enemyPos, 1.5f))
        {
            std::cout << "\nENCOUNTER TRIGGERED!\n";
            break;
        }

        std::cout << "\nMove (W/A/S/D) or Q to quit: ";
        char input;
        std::cin >> input;

        switch (input)
        {
        case 'w': playerPos.y += 1; break;
        case 's': playerPos.y -= 1; break;
        case 'a': playerPos.x -= 1; break;
        case 'd': playerPos.x += 1; break;
        case 'q': running = false; break;
        default: break;
        }
    }

    return 0;
}
