#pragma once
#include <wrl.h>
#include <dxcapi.h>
#include <unordered_map>
#include <string>


class DxCompile
{
	//PixelShaderData* pixelShaderData = nullptr;
	//VertexShaderData* vertexShaderData = nullptr;

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

	//Dxのコンパイラーを設定する
	void SetDXcCompiler();


public:
	void Initialize();

	[[nodiscard]] Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		std::string  const& folderPath_,
		std::string const& fileName_,
		const wchar_t* profile_);


};

