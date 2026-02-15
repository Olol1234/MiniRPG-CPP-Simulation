#pragma once

#include "Components.h"

class PlayerMovementSystem
{
public:
	static void HandleInputMove(PositionComponent& pos, char input, float step = 1.0f)
	{
		switch (input)
		{
			case 'w': pos.y += step; break;
			case 's': pos.y -= step; break;
			case 'a': pos.x -= step; break;
			case 'd': pos.x += step; break;
			//case 'q': return false;
			default: break;// ignore unknown keys
		}
		//return true;
	}
};