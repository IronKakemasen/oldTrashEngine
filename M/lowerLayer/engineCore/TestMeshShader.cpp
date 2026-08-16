#include "TestMeshShader.h"
#include "WinApp.h"


void TestMeshShader::CompileShaders()
{
	srcMeshShaderBlob = WinApp::dxCompile.CompileShader("SampleMS/", "SampleMeshShader", L"ms_6_5");

	srcPixelShaderBlob = WinApp::dxCompile.CompileShader("SampleMS/", "Sample.PS", L"ps_6_0");
	
	srcMeshShaderBlob2 = WinApp::dxCompile.CompileShader("SampleMS/", "Model.MS", L"ms_6_5");
	srcPixelShaderBlob2 = WinApp::dxCompile.CompileShader("SampleMS/", "Model.PS", L"ps_6_0");
}

void TestMeshShader::Create2()
{
	auto* device = WinApp::deviceSetUp.Getter_Device();

	staticSampler->MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
	staticSampler->MaxAnisotropy = 1;
	staticSampler->ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSampler->BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	staticSampler->MinLOD = 0.0f;
	staticSampler->MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler->ShaderRegister = 0;
	staticSampler->RegisterSpace = 0;
	staticSampler->ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	staticSampler->Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler->AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler->AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler->AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
	ranges.resize(6);

	for (int i = 0;i < 5; ++i)
	{
		ranges[i].BaseShaderRegister = i;
		//数は1つ
		ranges[i].NumDescriptors = 1;
		//SRVを使う
		ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		//offsetを自動計算
		ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		heapRootparams[i] = {};

		//Descriptortableを使う
		heapRootparams[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//pixcelShaderを使う
		if (i <= 3)
		{
			heapRootparams[i].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_MESH;
		}
		else 
		{
			heapRootparams[i].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		}

		//tableの中身の配列を指定
		heapRootparams[i].DescriptorTable.pDescriptorRanges = &ranges[i];
		//tableで利用する
		heapRootparams[i].DescriptorTable.NumDescriptorRanges = 1;
	}

	heapRootparams[5] = {};
	heapRootparams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	heapRootparams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_MESH;
	heapRootparams[5].Descriptor.ShaderRegister = 0;

	signatureDesc->pParameters = heapRootparams;
	signatureDesc->NumParameters = 6;
	signatureDesc->pStaticSamplers = staticSampler.get();
	signatureDesc->NumStaticSamplers = 1;
	signatureDesc->Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(
		signatureDesc.get(),
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	//バイナリをもとにrootSignatureを作成
	hr = device->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature2.GetAddressOf()));

	delete[] heapRootparams;

	MeshShaderPipelineStateStream  pipelineDesc = {};

	//シェーダーのセット
	CD3DX12_SHADER_BYTECODE ms =
	{
		srcMeshShaderBlob2->GetBufferPointer(),
		srcMeshShaderBlob2->GetBufferSize()
	};

	CD3DX12_SHADER_BYTECODE ps =
	{
		srcPixelShaderBlob2->GetBufferPointer(),
		srcPixelShaderBlob2->GetBufferSize()
	};

	//ラスタライザー
	CD3DX12_RASTERIZER_DESC desc_rs = {};
	desc_rs.FillMode = D3D12_FILL_MODE_SOLID;
	desc_rs.CullMode = D3D12_CULL_MODE_BACK;
	desc_rs.FrontCounterClockwise = FALSE;
	desc_rs.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	desc_rs.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	desc_rs.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	desc_rs.DepthClipEnable = FALSE;
	desc_rs.MultisampleEnable = FALSE;
	desc_rs.AntialiasedLineEnable = FALSE;
	desc_rs.ForcedSampleCount = 0;
	desc_rs.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//レンダーターゲットのブレンダー設定
	CD3DX12_BLEND_DESC blendDesc = {};
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//深度ステート
	CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;

	//ブレンドモードに応じて書き込みをするか
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つかり近ければ描画される
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;

	//サンプラーのカウント
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	//レンダーターゲットのフォーマット
	CD3DX12_RT_FORMAT_ARRAY renderTargetFormatArray = {};
	renderTargetFormatArray.NumRenderTargets = 1;
	renderTargetFormatArray.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	pipelineDesc.RTVFormats = renderTargetFormatArray;
	pipelineDesc.MS = ms;
	pipelineDesc.PS = ps;
	pipelineDesc.BlendState = blendDesc;
	pipelineDesc.DepthStencilState = depthStencilDesc;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	pipelineDesc.RasterizerState = desc_rs;
	pipelineDesc.SampleDesc = sampleDesc;
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.pRootSignature = rootSignature2.Get();

	D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
	streamDesc.pPipelineStateSubobjectStream = &pipelineDesc;
	streamDesc.SizeInBytes = sizeof(pipelineDesc);

	//実際に生成
	hr = device->CreatePipelineState(&streamDesc,
		IID_PPV_ARGS(pipelineStateObject2.GetAddressOf()));

	assert(SUCCEEDED(hr));
}

void TestMeshShader::CreateRootSignature()
{	
	auto* device = WinApp::deviceSetUp.Getter_Device();

	staticSampler->MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
	staticSampler->MaxAnisotropy = 1;
	staticSampler->ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSampler->BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	staticSampler->MinLOD = 0.0f;
	staticSampler->MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler->ShaderRegister = 0;
	staticSampler->RegisterSpace = 0;
	staticSampler->ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	staticSampler->Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler->AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler->AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler->AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
	ranges.resize(2);
	
	for (int i = 0;i < 2; ++i)
	{
		ranges[i].BaseShaderRegister = i;
		//数は1つ
		ranges[i].NumDescriptors = 1;
		//SRVを使う
		ranges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		//offsetを自動計算
		ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		heapRootparams[i] = {};

		//Descriptortableを使う
		heapRootparams[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//pixcelShaderを使う
		heapRootparams[i].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_MESH;
		//tableの中身の配列を指定
		heapRootparams[i].DescriptorTable.pDescriptorRanges = &ranges[i];
		//tableで利用する
		heapRootparams[i].DescriptorTable.NumDescriptorRanges = 1;
	}

	heapRootparams[2] = {};
	heapRootparams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	heapRootparams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_MESH;
	heapRootparams[2].Descriptor.ShaderRegister = 0;

	signatureDesc->pParameters = heapRootparams;
	signatureDesc->NumParameters = 3;
	signatureDesc->pStaticSamplers = staticSampler.get();
	signatureDesc->NumStaticSamplers = 1;
	signatureDesc->Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(
		signatureDesc.get(),
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	//バイナリをもとにrootSignatureを作成
	hr = device->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(),signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature.GetAddressOf()));

	delete[] heapRootparams;
}

void TestMeshShader::CreateBuffers()
{
	auto* device = WinApp::deviceSetUp.Getter_Device();

	vertexStructuredBuffer.vertex.CreateAndMapping(device, 3);
	vertexStructuredBuffer.vertex.gpuHandleIndex = 
		WinApp::srvCreator.Getter_StBufferCretaor()->CreateSRVForStructuredBuffer(3, sizeof(Vertex2), vertexStructuredBuffer.vertex.shaderBuffer);
	vertexStructuredBuffer.vertex.buffMap[0].position = { -1.0f , -1.0f , 10.0f ,0.0f};
	vertexStructuredBuffer.vertex.buffMap[1].position = { 0.0f, 1.0f, 10.0f ,0.0f };
	vertexStructuredBuffer.vertex.buffMap[2].position = { 1.0f , -1.0f , 10.0f ,0.0f };

	indexStructuredBuffer.indices.CreateAndMapping(device, 3);
	indexStructuredBuffer.indices.buffMap[0] = 0;
	indexStructuredBuffer.indices.buffMap[1] = 1;
	indexStructuredBuffer.indices.buffMap[2] = 2;
	indexStructuredBuffer.indices.gpuHandleIndex =
		WinApp::srvCreator.Getter_StBufferCretaor()->CreateSRVForStructuredBuffer(3, sizeof(uint32_t), indexStructuredBuffer.indices.shaderBuffer);

	transformMatrixBuffer.matrix.CreateAndMapping(device);
}


//PSOを作成する
void TestMeshShader::CreatePso()
{
	MeshShaderPipelineStateStream  pipelineDesc = {};
	auto* device = WinApp::deviceSetUp.Getter_Device();

	//シェーダーのセット
	CD3DX12_SHADER_BYTECODE ms =
	{
		srcMeshShaderBlob->GetBufferPointer(),
		srcMeshShaderBlob->GetBufferSize()
	};

	CD3DX12_SHADER_BYTECODE ps =
	{
		srcPixelShaderBlob->GetBufferPointer(),
		srcPixelShaderBlob->GetBufferSize()
	};
	
	//ラスタライザー
	CD3DX12_RASTERIZER_DESC desc_rs = {};
	desc_rs.FillMode = D3D12_FILL_MODE_SOLID;
	desc_rs.CullMode = D3D12_CULL_MODE_NONE;
	desc_rs.FrontCounterClockwise = FALSE;
	desc_rs.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	desc_rs.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	desc_rs.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	desc_rs.DepthClipEnable = FALSE;
	desc_rs.MultisampleEnable = FALSE;
	desc_rs.AntialiasedLineEnable = FALSE;
	desc_rs.ForcedSampleCount = 0;
	desc_rs.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//レンダーターゲットのブレンダー設定
	CD3DX12_BLEND_DESC blendDesc = {};
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//深度ステート
	CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;

	//ブレンドモードに応じて書き込みをするか
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つかり近ければ描画される
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;

	//サンプラーのカウント
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	//レンダーターゲットのフォーマット
	CD3DX12_RT_FORMAT_ARRAY renderTargetFormatArray = {};
	renderTargetFormatArray.NumRenderTargets = 1;
	renderTargetFormatArray.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	pipelineDesc.RTVFormats = renderTargetFormatArray;
	pipelineDesc.MS = ms;
	pipelineDesc.PS = ps;
	pipelineDesc.BlendState = blendDesc;
	pipelineDesc.DepthStencilState = depthStencilDesc;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	pipelineDesc.RasterizerState = desc_rs;
	pipelineDesc.SampleDesc = sampleDesc;
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.pRootSignature = rootSignature.Get();

	D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
	streamDesc.pPipelineStateSubobjectStream = &pipelineDesc;
	streamDesc.SizeInBytes = sizeof(pipelineDesc);

	//実際に生成
	HRESULT hr = device->CreatePipelineState(&streamDesc,
		IID_PPV_ARGS(pipelineStateObject.GetAddressOf()));

	assert(SUCCEEDED(hr));

}

TestMeshShader::TestMeshShader()
{
	staticSampler.reset(new D3D12_STATIC_SAMPLER_DESC);
	signatureDesc.reset(new D3D12_ROOT_SIGNATURE_DESC);
	heapRootparams = new D3D12_ROOT_PARAMETER[3];
}

void TestMeshShader::Init()
{
	CreateBuffers();
	CompileShaders();
	CreateRootSignature();
	CreatePso();

	blockModel.reset(new BlockModel);
	blockModel->Init(nullptr);
	signatureDesc.release();
	signatureDesc.reset(new D3D12_ROOT_SIGNATURE_DESC);
	heapRootparams = new D3D12_ROOT_PARAMETER[6];
	Create2();
}
