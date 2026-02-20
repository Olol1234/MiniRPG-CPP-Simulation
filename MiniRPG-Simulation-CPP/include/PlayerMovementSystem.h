#pragma once

#include "Components.h"

class PlayerMovementSystem
{
public:
	static void HandleRealtimeInput(
		PositionComponent& pos,
		float speed,
		float dt
	);
};