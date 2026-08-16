#pragma once

#include <string>
#include <unordered_map>
#include <d3d12.h>

struct IDxcBlob;

struct ShaderSet
{
	IDxcBlob* verShaderBlob = nullptr;
	IDxcBlob* pixShaderBlob = nullptr;

};

class VPShaderTable
{
	std::unordered_map<std::string, int> map_nameAndID;
	std::vector<std::string> nameList;
	std::unordered_map<std::string, ShaderSet> table;
	int setIndex = 0;



public:

	inline auto& Getter_Map_nameAndID()
	{
		return map_nameAndID;
	}


	inline auto& Getter_NameList()
	{
		return nameList;
	}

	int GetIDFromTableName(std::string shaderSetName_);
	ShaderSet Getter_ShaderSet(std::string name_);
	std::unordered_map<std::string, ShaderSet> Getter_ShaderTable();

	//シェーダセットのテーブルを設定
	void AddToTable(std::string name_,
		IDxcBlob* src_vertexShaderBlob_, IDxcBlob* src_pixelShaderBlob_);

};
