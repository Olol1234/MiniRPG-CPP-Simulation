#pragma once

#include "Definitions.h"

struct HealthComponent
{
    int hp;
};

struct AttackComponent
{
    int damage;
};

struct PositionComponent
{
    float x;
    float y;
};

struct WorldEnemy
{
    PositionComponent position;
    const CharacterDefinition* enemyDef;
};