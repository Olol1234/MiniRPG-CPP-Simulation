#pragma once

#include "Components.h"

class EncounterSystem
{
    public:
        static bool CheckEncounter(
            const PositionComponent& playerPos,
            const PositionComponent& enemyPos,
            float triggerDistance
        );
};