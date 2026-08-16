#include "VPShaderTable.h"
#include "../shaderData/vertexShaderData/vertexShaderData.h"
#include "../shaderData/pixelShaderData/pixelShaderData.h"
#include "../../../../utilities/benriTemplateFunc/benriTempFunc.h"
#include <assert.h>

#pragma comment(lib,"d3d12.lib")

int VPShaderTable::GetIDFromTableName(std::string shaderSetName_)
{
	assert(DoesContain(nameList, shaderSetName_));

	return map_nameAndID[shaderSetName_];
}



//シェーダセットのテーブルを設定
void VPShaderTable::AddToTable(std::string name_,
	IDxcBlob* src_vertexShaderBlob_, IDxcBlob* src_pixelShaderBlob_)
{
	table[name_].verShaderBlob = src_vertexShaderBlob_;
	table[name_].pixShaderBlob = src_pixelShaderBlob_;

	map_nameAndID[name_] = setIndex;
	nameList.emplace_back(name_);

	setIndex++;
}

ShaderSet VPShaderTable::Getter_ShaderSet(std::string name_)
{

	assert(DoesContain(nameList, name_));

	return table[name_];
}


std::unordered_map<std::string, ShaderSet> VPShaderTable::Getter_ShaderTable()
{
	return table;
}