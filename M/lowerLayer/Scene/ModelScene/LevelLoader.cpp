#include "LevelLoader.h"
#include <fstream>
#include <iostream>

SceneData LevelLoader::Load(const std::string& filePath)
{
    SceneData sceneData;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[LevelLoader] エラー: ファイルが開けませんでした -> " << filePath << std::endl;
        return sceneData;
    }

    nlohmann::json deserialized;
    try {
        file >> deserialized;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "[LevelLoader] JSONパースエラー: " << e.what() << std::endl;
        return sceneData;
    }

    // 【修正箇所】"name" の文字列判定を外し、"objects" 配列があるかだけをチェック
    if (deserialized.contains("objects") && deserialized["objects"].is_array()) {
        sceneData.allObj.reserve(deserialized["objects"].size());

        for (const auto& objJson : deserialized["objects"]) {
            ParseObject(objJson, sceneData.allObj);
        }
    }
    else {
        std::cerr << "[LevelLoader] 警告: 'objects' 配列が見つかりません。" << std::endl;
    }

    return sceneData;
}
void LevelLoader::ParseObject(const nlohmann::json& objJson, std::vector<OBJ>& outObjects)
{
    OBJ obj;

    // オブジェクト名
    if (objJson.contains("name")) {
        obj.objName = objJson["name"].get<std::string>();
    }

    // ファイル名（パス）
    if (objJson.contains("file_name")) {
        obj.filePath = objJson["file_name"].get<std::string>();
    }

    // メッシュ種別（MESHなど）
    if (objJson.contains("type")) {
        obj.meshType = objJson["type"].get<std::string>();
    }

    // コライダー種別
    if (objJson.contains("collider") && objJson["collider"].contains("type")) {
        obj.colliderType = objJson["collider"]["type"].get<std::string>();
    }

    // Transform解析（座標系変換含む）
    if (objJson.contains("transform")) {
        const auto& transform = objJson["transform"];

        // 位置: Blender (X, Y, Z) -> ゲーム (X, Z, Y)
        if (transform.contains("translation") && transform["translation"].is_array() && transform["translation"].size() >= 3) {
            obj.pos.x = transform["translation"][0].get<float>();
            obj.pos.y = transform["translation"][2].get<float>();
            obj.pos.z = transform["translation"][1].get<float>();
        }

        // 回転: Blender (X, Y, Z) -> ゲーム (-X, -Z, -Y) 符号反転
        if (transform.contains("rotation") && transform["rotation"].is_array() && transform["rotation"].size() >= 3) {
            obj.rotation.x = -transform["rotation"][0].get<float>();
            obj.rotation.y = -transform["rotation"][2].get<float>();
            obj.rotation.z = -transform["rotation"][1].get<float>();
        }

        // スケール: Blender (X, Y, Z) -> ゲーム (X, Z, Y)
        if (transform.contains("scaling") && transform["scaling"].is_array() && transform["scaling"].size() >= 3) {
            obj.scale.x = transform["scaling"][0].get<float>();
            obj.scale.y = transform["scaling"][2].get<float>();
            obj.scale.z = transform["scaling"][1].get<float>();
        }
    }

    // 結果に追加
    outObjects.push_back(obj);

    // 子要素（children）があれば再帰解析
    if (objJson.contains("children") && objJson["children"].is_array()) {
        for (const auto& childJson : objJson["children"]) {
            ParseObject(childJson, outObjects);
        }
    }
}