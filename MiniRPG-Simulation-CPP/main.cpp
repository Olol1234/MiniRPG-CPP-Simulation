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

    while (gameState != GameState::Exit)
    {
        std::cout << "\nPlayer Position: (" << playerPos.x << ", " << playerPos.y << ")\n";
        for (int i = 0; i < enemies.size(); i++)
        {
            float dist = Distance(
                playerPos.x,
                playerPos.y,
                enemies[i].position.x,
                enemies[i].position.y
            );
            std::cout << "Distance to Enemy " << i << ": " << dist << "\n";
            if (EncounterSystem::CheckEncounter(playerPos, enemies[i].position, 1.5f))
            {
                std::cout << "\n ENCOUNTER WITH ENEMY " << i << "!\n";
                CombatSystem::StartCombat(playerStats, enemies[i].enemyDef);
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
