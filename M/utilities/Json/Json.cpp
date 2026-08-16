#include "Json.h"
#include <fstream>
#include <iostream>
#include <filesystem>

std::unordered_map<std::string, nlohmann::json> Json::dataMap;


bool Json::Load(const std::string& path)
{
    if (dataMap.find(path) != dataMap.end())
    {
        // すでに読み込まれている場合はスキップ
        return true;
	}

    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
		std::cout << "Json::Load: Failed to open file " << path << std::endl;
        return false;
    }

    nlohmann::json j;
    ifs >> j;

    dataMap[path] = j;
    return true;
}

void Json::LoadAll(const std::string& directoryPath)
{
    std::filesystem::create_directories(directoryPath);
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
    {
        if (entry.path().extension() == ".json")
        {
            Load(entry.path().generic_string());
        }
    }
}


bool Json::Save(const std::string& path)
{
    auto it = dataMap.find(path);
    if (it == dataMap.end())
    {
		std::cout << "Json::Save: No data for path " << path << std::endl;
        return false;
    }

    std::filesystem::path p(path);
    std::filesystem::create_directories(p.parent_path());
    
    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
		std::cout << "Json::Save: Failed to open file " << path << " for writing." << std::endl;
        return false;
    }

    ofs << it->second.dump(4);
	std::cout << "Json::Save: Saved data to " << path << std::endl;
    return true;
}

void Json::SaveAll()
{
    for (auto& [path, json] : dataMap)
    {
        Save(path);
    }
}