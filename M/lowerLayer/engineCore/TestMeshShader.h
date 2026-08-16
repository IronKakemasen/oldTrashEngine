#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxcapi.h>
#include <memory>
#include "./Buffer/StructuredBuffer/IndexStructuredBuffer/IndexStructuredBuffer.h"
#include "./Buffer/StructuredBuffer/VertexStructuredBuffer/VertexStructuredBuffer.h"
#include "./Buffer/constantBuffer/TransformMatrixBuffer/TransformMatrixBuffer.h"
#include "../../utilities/Transform/Transform.h"
#include "Mesh/meshStructure/ModelData/ModelData.h"
#include "../Scene/ModelScene/AAA/BlockModel.h"


struct MeshShaderPipelineStateStream 
{
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_MS MS;
	CD3DX12_PIPELINE_STATE_STREAM_PS PS;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RasterizerState;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendState;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DepthStencilState;
	CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
	CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK SampleMask;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
} ;

struct TestMeshShader
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineStateObject = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	std::unique_ptr<D3D12_STATIC_SAMPLER_DESC> staticSampler;
	std::unique_ptr<D3D12_ROOT_SIGNATURE_DESC> signatureDesc = {};

	D3D12_ROOT_PARAMETER* heapRootparams;

	Microsoft::WRL::ComPtr<IDxcBlob> srcMeshShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> srcPixelShaderBlob;

	IndexStructuredBuffer indexStructuredBuffer;
	VertexStructuredBuffer vertexStructuredBuffer;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineStateObject2 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature2 = nullptr;
	std::unique_ptr<D3D12_ROOT_SIGNATURE_DESC> signatureDesc2 = {};

	Microsoft::WRL::ComPtr<IDxcBlob> srcMeshShaderBlob2;
	Microsoft::WRL::ComPtr<IDxcBlob> srcPixelShaderBlob2;

	TransformMatrixBuffer2 transformMatrixBuffer;
	Transform trans;

	std::unique_ptr<BlockModel> blockModel;

	TestMeshShader();
	void Init();
	void Update();

private:
	void CreateBuffers();
	void CreatePso();
	void CompileShaders();
	void CreateRootSignature();
	void Create2();

};