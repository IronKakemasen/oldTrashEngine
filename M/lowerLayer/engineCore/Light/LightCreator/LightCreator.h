#pragma once
#include "../../Buffer/constantBuffer/DirectionalLightBuffer/DirectionalLightBuffer.h"
#include "../../Buffer/StructuredBuffer/PointLightStructuredBuffer/PointLightStructuredBuffer.h"
#include "../../Buffer/StructuredBuffer/RectLightStructuredBuffer/RectLightStructuredBuffer.h"

#include <memory>

class ExclusiveDraw;
class StructuredBufferSrvCreator;

class LightCreator
{
public:
	enum Max
	{
		kPLightMax = 15,
		kRLightmax = 3
	};

private:
	std::pair<std::unique_ptr < DirectionalLightBuffer >,
		std::unique_ptr < DirectionalLight > > dirLightSet;
	//ポイントライト
	std::unique_ptr < PointLightStructuredBuffer > pLightStructuredBuffer;
	std::vector<std::unique_ptr<PointLight>> pointLightContainer;
	//pointLightのsrvのインデックス
	uint16_t pointLightSrvIndex;

	//レクトライト
	std::unique_ptr < RectLightStructuredBuffer > rectLightStructuredBuffer;
	std::vector<std::unique_ptr<RectLight>> rectLightContainer;
	//srvのインデックス
	uint16_t rectLightSrvIndex;

	void CreateRectLight(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
		StructuredBufferSrvCreator* srvCreator_);
	void CreatePointLight(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
		StructuredBufferSrvCreator* srvCreator_);
	void CreateDirectionalLight(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_);
	
public:
	LightCreator();
	void Init(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_, 
		StructuredBufferSrvCreator* srvCreator_);
	inline auto* Getter_DirLight()
	{
		return dirLightSet.second.get();
	}
	inline auto* Getter_DirLightBuffer()
	{
		return dirLightSet.first.get();
	}

	inline auto* Getter_PointLightContainer()
	{
		return &pointLightContainer;
	}

	inline auto* Getter_PointLightStructuredBuffer()
	{
		return pLightStructuredBuffer.get();
	}

	inline auto* Getter_RectLightContainer()
	{
		return &rectLightContainer;
	}

	inline auto* Getter_RectLightStructuredBuffer()
	{
		return rectLightStructuredBuffer.get();
	}

};

