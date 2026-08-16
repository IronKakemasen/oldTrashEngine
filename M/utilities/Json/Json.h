#pragma once
#include "json.hpp"
#include "../../lowerLayer/math/matrix/matrix.h"
#include "../../lowerLayer/math/vector/vector.h"

class Json
{
public:
	/// <summary>
	/// dataMapにパラメータを追加する（key は "/a/b/c" のような階層パス対応）
	/// </summary>
	/// <typeparam name="T"> 値の型 </typeparam>
	/// <param name="path"> ファイルパス </param>
	/// <param name="key"> キー </param>
	/// <param name="value"> 値 </param>
	template<typename T>
	static void SaveParam(const std::string& path, const std::string& key, const T& value)
	{
		std::string fixedKey = key;
		if (fixedKey[0] != '/')fixedKey = "/" + fixedKey;

		auto& root = dataMap[path];
		nlohmann::json::json_pointer jp(fixedKey);
		root[jp] = value;
	}

	/// <summary>
	/// dataMapからパラメータを取得する（key は "/a/b/c" のような階層パス対応）
	/// </summary>
	/// <typeparam name="T"> 値の型 </typeparam>
	/// <param name="path"> ファイルパス </param>
	/// <param name="key"> キー </param>
	/// <param name="outValue"> 取得した値の出力先 </param>
	/// <returns> 成否 </returns>
	template<typename T>
	static bool LoadParam(const std::string& path, const std::string& key, T& outValue)
	{
		std::string fixedKey = key;
		if (fixedKey[0] != '/')fixedKey = "/" + fixedKey;

		auto it = dataMap.find(path);
		if (it == dataMap.end()) return false;

		nlohmann::json::json_pointer jp(fixedKey);

		if (!it->second.contains(jp)) return false;

		outValue = it->second.at(jp).get<T>();
		return true;
	}

	// ファイルを読み込んで dataMap[path] に保存
	static bool Load(const std::string& path); 
	// そのディレクトリの全てのファイルを読み込んで dataMap に保存
	static void LoadAll(const std::string& directoryPath);
	// dataMap[path] の内容をファイルに保存
	static bool Save(const std::string& path);
	// dataMap の全ての内容を対応するファイルに保存
	static void SaveAll();


private:
	static std::unordered_map<std::string, nlohmann::json> dataMap;
};

inline void to_json(nlohmann::json& j, const Vector2& v)
{
	j = nlohmann::json{ {"x", v.x}, {"y", v.y} };
}

inline void from_json(const nlohmann::json& j, Vector2& v)
{
	j.at("x").get_to(v.x);
	j.at("y").get_to(v.y);
}

inline void to_json(nlohmann::json& j, const Vector3& v)
{
	j = nlohmann::json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
}

inline void from_json(const nlohmann::json& j, Vector3& v)
{
	j.at("x").get_to(v.x);
	j.at("y").get_to(v.y);
	j.at("z").get_to(v.z);
}

inline void to_json(nlohmann::json& j, const Vector4& v)
{
	j = nlohmann::json{ {"x", v.x}, {"y", v.y}, {"z", v.z},{"w", v.w} };
}

inline void from_json(const nlohmann::json& j, Vector4& v)
{
	j.at("x").get_to(v.x);
	j.at("y").get_to(v.y);
	j.at("z").get_to(v.z);
	j.at("w").get_to(v.w);
}

inline void to_json(nlohmann::json& j, const Matrix4& m)
{
	j = nlohmann::json{
	{ "m", nlohmann::json::array() }
	};

	for (int i = 0; i < 4; ++i)
	{
		j["m"].push_back({ m.m[i][0], m.m[i][1], m.m[i][2], m.m[i][3] });
	}
}
inline void from_json(const nlohmann::json& j, Matrix4& m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			j.at("m").at(i).at(k).get_to(m.m[i][k]);
		}
	}
}
