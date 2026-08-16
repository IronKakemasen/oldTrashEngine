#pragma once
#include "./shaderData/vertexShaderData/vertexShaderData.h"
#include "./shaderData/pixelShaderData/pixelShaderData.h"
#include "./vpShaderTable/vpShaderTable.h"

class DxCompile;

class VpShaders
{
	DxCompile* shaderCompiler = nullptr;

	//[ ShaderData ]
	VertexShaderData vertexShaderData;
	PixelShaderData pixelShaderData;
	//[ vpShadertable ]
	VPShaderTable vpShaderTable;

	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(std::string folderPath_, std::string fileName_, const wchar_t* profile_);

public:

	auto* Getter_VertexShaderData()
	{
		return &vertexShaderData;
	}

	auto* Getter_PixelShaderData()
	{
		return &pixelShaderData;
	}

	auto* Getter_VPShaderTable()
	{
		return &vpShaderTable;
	}



	void Init(DxCompile* shaderCompiler_);
	void AddPixelShader(std::string folderPath_ ,std::string fileName_);
	void AddVertexShader(std::string folderPath_, std::string fileName_);
	void AddToTable(std::string shaderSetName_, std::string pixName_, std::string vertexName_);



};