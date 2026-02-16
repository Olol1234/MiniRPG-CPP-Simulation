#include "DefinitionManager.h"
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

bool DefinitionManager::LoadCharacterDef(
    const std::string& id,
    const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    json j;
    file >> j;

    CharacterDefinition def;
    def.name = j["name"];
    def.baseMaxHP = j["baseMaxHP"];
    def.baseAttack = j["baseAttack"];
    def.baseDefense = j["baseDefense"];

    characterDefinition[id] = def;

    return true;
}

const CharacterDefinition* DefinitionManager::GetCharacterDef(
    const std::string& id) const
{
    auto it = characterDefinition.find(id);
    if (it == characterDefinition.end())
        return nullptr;

    return &it->second;
}
