#pragma once
#include <wrl.h>
#include <dxcapi.h>
#include <unordered_map>
#include <string>


struct ShaderDataBehavior
{
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDxcBlob>> data;

};