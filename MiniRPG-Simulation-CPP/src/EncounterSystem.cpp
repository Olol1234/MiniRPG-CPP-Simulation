#include "EncounterSystem.h"
#include "MathUtils.h"

bool EncounterSystem::CheckEncounter(
    const PositionComponent& playerPos,
    const PositionComponent& enemyPos,
    float triggerDistance
)
{
    float distance = Distance(
        playerPos.x,
        playerPos.y,
        enemyPos.x,
        enemyPos.y
    );

    return distance < triggerDistance;
}