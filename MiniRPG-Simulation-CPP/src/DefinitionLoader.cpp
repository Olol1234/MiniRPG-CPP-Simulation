#include "DefinitionLoader.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

CharacterDefinition LoadCharacterDefinition(const std::string& filePath)
{
    std::ifstream file(filePath);
    json j;
    file >> j;

    CharacterDefinition def;
    def.name = j["name"];
    def.baseMaxHP = j["baseMaxHP"];
    def.baseAttack = j["baseAttack"];
    def.baseDefense = j["baseDefense"];

    return def;
}
