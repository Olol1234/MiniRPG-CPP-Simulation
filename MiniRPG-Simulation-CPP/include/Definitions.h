#pragma once

#include <string>
#include <SFML/Graphics.hpp>

struct CharacterDefinition
{
	std::string name;
	int baseMaxHP;
	int baseAttack;
	int baseDefense;

	std::string spritePath;
	sf::Texture texture;
};