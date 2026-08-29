#pragma once
#include "json.hpp"
#include "Transform.h"


struct OBJ
{
    std::string objName;
    std::string filePath;
    std::string meshType;
    std::string colliderType;
    Vector3 pos;
    Vector3 rotation;
    Vector3 scale;
};

struct SceneData
{
    std::vector<OBJ> allObj;
};

struct LevelLoader
{
    static SceneData Load(const std::string& filePath);
    static void ParseObject(const nlohmann::json& objJson, std::vector<OBJ>& outObjects);

};




