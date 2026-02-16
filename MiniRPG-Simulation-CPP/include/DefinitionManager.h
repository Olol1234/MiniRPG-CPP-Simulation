#pragma once

#include "Definitions.h"
#include <unordered_map>

class DefinitionManager
{
public:
	bool LoadCharacterDef(const std::string& id, const std::string& file);
	const CharacterDefinition* GetCharacterDef(const std::string& id) const;

private:
	std::unordered_map<std::string, CharacterDefinition> characterDefinition;
};
