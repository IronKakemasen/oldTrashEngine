#include "vpShaders.h"
#include <assert.h>
#include "../WinApp.h"
#include "../Essential/dxCompile/dxCompile.h"

#pragma comment(lib,"d3d12.lib")


void VpShaders::AddToTable(std::string shaderSetName_, std::string pixName_, std::string vertexName_)
{
	vpShaderTable.AddToTable(shaderSetName_, vertexShaderData.data[vertexName_].Get(),
		pixelShaderData.data[pixName_].Get());
}

Microsoft::WRL::ComPtr<IDxcBlob> VpShaders::CompileShader(std::string folderPath_, std::string fileName_, const wchar_t* profile_)
{
	return shaderCompiler->CompileShader(folderPath_,fileName_, profile_);
}


void VpShaders::Init(DxCompile* shaderCompiler_)
{
	shaderCompiler = shaderCompiler_;

}


void VpShaders::AddPixelShader(std::string folderPath_ , std::string fileName_)
{
	auto profile = L"ps_6_0";

	pixelShaderData.data[fileName_] = CompileShader(folderPath_,fileName_, profile);
}

void VpShaders::AddVertexShader(std::string folderPath_, std::string fileName_)
{
	auto profile = L"vs_6_0";
	vertexShaderData.data[fileName_] = CompileShader(folderPath_,fileName_, profile);

}


