#include "PlayerMovementSystem.h"
#include "Components.h"
#include <algorithm>

void PlayerMovementSystem::HandleRealtimeInput(
	PositionComponent& pos,
	float speed,
	float dt
)
{
	PositionComponent newPos = pos;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
		newPos.y -= speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
		newPos.y += speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		newPos.x -= speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		newPos.x += speed * dt;

	const float MIN_X = -20.f;
	const float MAX_X = 20.f;
	const float MIN_Y = -15.f;
	const float MAX_Y = 15.f;

	newPos.x = std::clamp(newPos.x, MIN_X, MAX_X);
	newPos.y = std::clamp(newPos.y, MIN_Y, MAX_Y);

	pos = newPos;
}