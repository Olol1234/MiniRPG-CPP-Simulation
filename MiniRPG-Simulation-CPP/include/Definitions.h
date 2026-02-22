#pragma once

#include <string>
#include <SFML/Graphics.hpp>

struct AttackInfo
{
	std::string name;
	int damage;
	std::string element;
};

struct CharacterDefinition
{
	std::string name;
	int baseMaxHP;
	//int baseAttack;
	int baseDefense;

	std::string spritePath;
	sf::Texture texture;

	std::vector<AttackInfo> attacks;
};
