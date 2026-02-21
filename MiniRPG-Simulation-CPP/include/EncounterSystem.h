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
        static bool CheckExitDoorEncounter(
            const PositionComponent& playerPos,
            float doorX,
            float doorY,
            float triggerDistance
		);
};