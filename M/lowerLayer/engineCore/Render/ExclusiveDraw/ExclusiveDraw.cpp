#include "ExclusiveDraw.h"
#include "../../PSO/allPipelineSet.h"
#include "../../Mesh/allMesh/allMesh.h"
#include "../../Buffer/gpuResources/Data/ShaderBufferData/ShaderBufferData.h"
#include "../../Essential/BarrierControl/BarrierControl.h"
#include "../../Buffer/constantBuffer/DirectionalLightBuffer/DirectionalLightBuffer.h"
#include "../../Buffer/constantBuffer/CameraParaBuffer/CameraParaBuffer.h"
#include "../../../M.h"
#include "../Palette/Palette.h"
#include "../../Buffer/constantBuffer/Time/TimeConstBuffer.h"
#include "../../Buffer/constantBuffer/EnviornmentBuffer/EnviornmentBuffer.h"
#include "../../Buffer/constantBuffer/CameraViewMat/CameraViewMat.h"

#include "imgui.h"
#include "../Render/OffScreenManager/OffScreenManager.h"
#include "../../Camera/CameraController/CameraController.h"


void ExclusiveDraw::Setter_PLightSrvIndex(uint16_t* pLightSrvIndex_)
{
	pLightSrvIndex = pLightSrvIndex_;
}

void ExclusiveDraw::Setter_RLightSrvIndex(uint16_t* rLightSrvIndex_)
{
	rLightSrvIndex = rLightSrvIndex_;
}

void ExclusiveDraw::Setter_DirectionalLightBuffer(DirectionalLightBuffer* dirLightBuffer_)
{
	dirLightBuffer = dirLightBuffer_;
}

void ExclusiveDraw::DrawEllipseWireFrame(Vector3 center_, float radius_, Vector3 rotation_,
	Vector4 color_, Matrix4* vpMat_)
{
	static float const div = 40.0f;
	static float const deg = 360.0f / div;

	for(float i = -2.0f;i< deg; ++i)
	{
		float stRad = GetRadian(div * i);
		float edRad = GetRadian(div * (i + 1));

		Vector3 st = { cosf(stRad), sinf(stRad) ,0.0f};
		Vector3 ed = { cosf(edRad), sinf(edRad) ,0.0f };

		float tmp = radius_;
		Matrix4 rtMat = Get_SRTMat3D({ tmp ,tmp ,1.0f }, rotation_, center_);

		st = st.GetMultiply(rtMat);
		ed = ed.GetMultiply(rtMat);

		DrawLine(st, ed, color_, vpMat_);
	}

}

void ExclusiveDraw::DrawLine(Vector3 st_, Vector3 ed_, Vector4 color_, Matrix4* vpMat_)
{
	auto* lineMesh = allMesh->Getter_LineMesh();
	lineMesh->DetectOverDrawing();
	int const vertexCnt = 2;
	UINT i = lineMesh->GetCurrentIndex();
	uint32_t const usingVertex_index = i * vertexCnt;

	Vector3 vertices[2] = { st_,ed_ };

	float const i255 = CommonV::inv_255;
	Vector4 color = { color_.x * i255,color_.y * i255,color_.z * i255,color_.w * i255 };

	//< データの入力 >
	//[ 頂点 ]
	std::memcpy(&lineMesh->GetVertexMap()[usingVertex_index], vertices, sizeof(Vector3) * vertexCnt);

	//[ 行列 ]
	lineMesh->SetViewProjectionMatrix(vpMat_);

	//[ マテリアル ]
	//色
	lineMesh->SetMaterial(&color, i);

	//< データの転送 >
	static int shaderSet = 
		M::GetInstance()->GetShaderSetIndexFromFileName("Line.VS", "Line.PS");

	auto* src_pipeline = allPipelineSet->Getter_pipelineSet(shaderSet, BlendMode::kBlendModeAdd, CullMode::kCullModeNone, OffScreenManager::curRenderTargetStatus);

	auto* cList = src_pipeline->Getter_CommandList();

	cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	src_pipeline->SetGraphicsRootSignature();
	src_pipeline->SetPipelineState();

	//VBV
	cList->IASetVertexBuffers(0, 1, lineMesh->GetVertexBufferView());

	//Cバッファの場所を指定
	src_pipeline->SetConstantBufferViews(
		0,
		lineMesh->GetViewProjectionVirtualPtr(),
		lineMesh->GetMaterialVirtualPtr(i)
		);

	cList->DrawInstanced(vertexCnt, 1, static_cast<UINT>(usingVertex_index), 0);

	//次の描画用にインクリメント
	lineMesh->GetCurrentIndex()++;
}

void ExclusiveDraw::DrawModel(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_)
{

	int n = (int)meshAndData_->Getter_ModelData().resMesh.size();

	for (int i = 0; i < n; ++i)
	{
		auto* mesh = meshAndData_->Getter_MeshForModel(i);
		auto* appearance = &(*meshAndData_->GetAppearance())[i];
		auto* modelData = meshAndData_->Getter_ModelDataOfResMaterials(i);
		auto* src_pipeline = allPipelineSet->Getter_pipelineSet(appearance->shaderSetIndex, 
			appearance->blendMode, appearance->cullMode, OffScreenManager::curRenderTargetStatus);
		auto* cList = src_pipeline->Getter_CommandList();

		{
			char buf[256];
			sprintf_s(buf, "DrawModel: shaderSetIndex=%d curRenderTargetStatus=%d\n",
				appearance->shaderSetIndex, OffScreenManager::curRenderTargetStatus);
			M::GetInstance()->LogM(std::string(buf));
		}

		{
			char buf2[256];
			sprintf_s(buf2, "Pipeline commandList ptr (DrawModel) = %p\n", src_pipeline->Getter_CommandList());
			M::GetInstance()->LogM(std::string(buf2));
		}


		//Pipeline
		cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		src_pipeline->SetGraphicsRootSignature();
		src_pipeline->SetPipelineState();

		//VBV
		cList->IASetVertexBuffers(0, 1, &mesh->vertexBuffer.view);
		//IBV
		cList->IASetIndexBuffer(&mesh->indexBuffer.view);

		//[ 行列 ]
		Matrix4 wMat = appearance->trans.GetWorldMatrix();
		Matrix4 wvp = wMat.Multiply(*vpMat_);
		mesh->transformMatrixBuffer.matrix.buffMap->world = wMat;
		mesh->transformMatrixBuffer.matrix.buffMap->wvp = wvp;

		//[ material ] 
		Vector4 color = appearance->color * CommonV::inv_255;
		mesh->materialBuffer.material.buffMap->albedoColor = color;
		mesh->materialBuffer.material.buffMap->uvTransform = appearance->uvTrans.GetUVMat();
		mesh->materialBuffer.material.buffMap->metalic = appearance->metalic;
		mesh->materialBuffer.material.buffMap->roughness = appearance->roughness;

		//texture
		int k = 0;
		for (; k < (int)appearance->use_texHandles.size(); ++k)
		{
			cList->SetGraphicsRootDescriptorTable(k,
				shaderBufferData->gpuHandleContainer[appearance->use_texHandles[k]]);			
		}

		//pointLight
		cList->SetGraphicsRootDescriptorTable(k++,
			shaderBufferData->gpuHandleContainer[*pLightSrvIndex]);

		//areaLight
		cList->SetGraphicsRootDescriptorTable(k++,
			shaderBufferData->gpuHandleContainer[*rLightSrvIndex]);

		auto* a = EnviornmentBuffer::Get();

		auto m = *CameraViewMat::Get()->viewMat.buffMap;

		*CameraViewMat::Get()->viewMat.buffMap = CameraController::curUsingViewMat;


		//Cバッファの場所を指定
		src_pipeline->SetConstantBufferViews(
			k,
			mesh->transformMatrixBuffer.matrix.GetVirtualGPUAddress(),
			mesh->materialBuffer.material.GetVirtualGPUAddress(),
			dirLightBuffer->dirLight.GetVirtualGPUAddress(),
			cameraParaBuffer->cameraPara.GetVirtualGPUAddress(),
			a->enviornmentParam.GetVirtualGPUAddress(),
			CameraViewMat::Get()->viewMat.GetVirtualGPUAddress());

		//描画
		UINT indexCnt = (UINT)meshAndData_->Getter_ModelData().resMesh[i].indices.size();
		cList->DrawIndexedInstanced(indexCnt, 1, 0, 0,0);
	}
}

void ExclusiveDraw::DrawShaderToy(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_,float time_)
{
	int n = (int)meshAndData_->Getter_ModelData().resMesh.size();

	for (int i = 0; i < n; ++i)
	{
		auto* mesh = meshAndData_->Getter_MeshForModel(i);
		auto* appearance = &(*meshAndData_->GetAppearance())[i];
		auto* modelData = meshAndData_->Getter_ModelDataOfResMaterials(i);
		auto* src_pipeline = allPipelineSet->Getter_pipelineSet(appearance->shaderSetIndex,
			BlendMode::kBlendModeNormal, CullMode::kCullModeNone, OffScreenManager::curRenderTargetStatus);
		auto* cList = src_pipeline->Getter_CommandList();

		//Pipeline
		cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		src_pipeline->SetGraphicsRootSignature();
		src_pipeline->SetPipelineState();

		//VBV
		cList->IASetVertexBuffers(0, 1, &mesh->vertexBuffer.view);
		//IBV
		cList->IASetIndexBuffer(&mesh->indexBuffer.view);

		//[ 行列 ]
		Matrix4 wMat = appearance->trans.GetWorldMatrix();
		Matrix4 wvp = wMat.Multiply(*vpMat_);
		mesh->transformMatrixBuffer.matrix.buffMap->world = wMat;
		mesh->transformMatrixBuffer.matrix.buffMap->wvp = wvp;

		//Cバッファの場所を指定
		src_pipeline->SetConstantBufferViews(
			0,
			mesh->transformMatrixBuffer.matrix.GetVirtualGPUAddress(),
			TimeConstBuffer::Get()->buffer.GetVirtualGPUAddress());

		//描画
		UINT indexCnt = (UINT)meshAndData_->Getter_ModelData().resMesh[i].indices.size();
		cList->DrawIndexedInstanced(indexCnt, 1, 0, 0, 0);
	}

}


void ExclusiveDraw::ResetDrawIndexes()
{
	allMesh->ResetDrawIndexes();
}

void ExclusiveDraw::Init(AllPipelineSet* allPipelineSet_, AllMesh* allMesh_, 
	ShaderBufferData* shaderBufferData_, CameraParaBuffer* cameraParaBuffer_)
{
	allMesh = allMesh_;
	allPipelineSet = allPipelineSet_;
	shaderBufferData = shaderBufferData_;
	cameraParaBuffer = cameraParaBuffer_;
}

void ExclusiveDraw::DrawSprite(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
	Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
	Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_)
{
	auto* quadMesh = allMesh->Getter_QuadMesh(); // 実関数名に合わせて Getter 呼び出しをそのまま使ってください

	quadMesh->DetectOverDrawing();

	int i = quadMesh->cur_drawIndex;

	uint32_t usingVertex_index = static_cast<uint32_t>(i) * static_cast<uint32_t>(quadMesh->vertexCnt);
	uint32_t usingIndex_index = static_cast<uint32_t>(i) * static_cast<uint32_t>(quadMesh->indexCnt);
	uint32_t totalVertices = static_cast<uint32_t>(quadMesh->vertexCnt) * static_cast<uint32_t>(quadMesh->kMaxDraw);
	uint32_t totalIndices = static_cast<uint32_t>(quadMesh->indexCnt) * static_cast<uint32_t>(quadMesh->kMaxDraw);

	//uint32_t const usingIndex_index = i * quadMesh->indexCnt;
	//uint32_t const usingVertex_index = i * quadMesh->vertexCnt;

	Vertex vData[4] =
	{
		leftBottom_,leftTop_,rightBottom_,rightTop_
	};

	uint32_t indices[6] = { 0,1,2,2,1,3 };

	float const i255 = CommonV::inv_255;
	Vector4 color = { color_.x * i255,color_.y * i255,color_.z * i255,color_.w * i255 };

	//< データの入力 >
	//[ インデックス ]
	std::memcpy(&quadMesh->indexMap[usingIndex_index], indices, sizeof(uint32_t) * quadMesh->indexCnt);

	//[ 頂点 ]
	std::memcpy(&quadMesh->vertexMap[usingVertex_index], vData, sizeof(Vertex) * quadMesh->vertexCnt);

	//[ 行列 ]
	Matrix4 wMat = trans_.GetWorldMatrix();
	Matrix4 wvp = wMat.Multiply(vpMat_);
	*quadMesh->wvpMatrixBuffer[i].matrix.buffMap = wvp;

	//[ マテリアル ]
	//色
	Matrix4 uvMat = uvTrans_.GetUVMat();
	quadMesh->materialBuffer[i].material.buffMap->albedoColor = color;
	quadMesh->materialBuffer[i].material.buffMap->uvTransform = uvMat;

	//< データの転送 >
	auto* src_pipeline = allPipelineSet->Getter_pipelineSet(shaderSet_, blendMode_, cullMode_, OffScreenManager::curRenderTargetStatus);
	auto* cList = src_pipeline->Getter_CommandList();

	cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	src_pipeline->SetGraphicsRootSignature();
	src_pipeline->SetPipelineState();

	//VBV
	cList->IASetVertexBuffers(0, 1, quadMesh->Getter_VertexBufferView());

	//IBV
	cList->IASetIndexBuffer(quadMesh->Getter_IndexBufferView());

	//texture
	cList->SetGraphicsRootDescriptorTable(0, shaderBufferData->gpuHandleContainer[texHandle_]);

	//Cバッファの場所を指定
	src_pipeline->SetConstantBufferViews(
		1,
		quadMesh->wvpMatrixBuffer[i].matrix.GetVirtualGPUAddress(),
		quadMesh->materialBuffer[i].material.GetVirtualGPUAddress()
	);

	//描画(DrawCall)。6インデックスで一つのインスタンス
	cList->DrawIndexedInstanced(quadMesh->indexCnt, 1, static_cast<UINT>(usingIndex_index), static_cast<UINT>(usingVertex_index), 0);

	//次の描画用にインクリメント
	quadMesh->cur_drawIndex++;

}

void ExclusiveDraw::DrawOnPalette(Palette* palette_)
{
	auto* paletteMesh = allMesh->Getter_MeshForPostEffect();

	int i = paletteMesh->curIndex;

	//使用する三角形のマップインデックス
	uint32_t const usingIndex_index = i * paletteMesh->kIndexCnt;
	uint32_t const usingVertex_index = i * paletteMesh->kVertexCnt;

	//< データの転送 >
	auto* src_pipeline = allPipelineSet->Getter_pipelineSet(palette_->WatchShaderSetIndex(),
		palette_->WatchBlendMode(), CullMode::kCullModeNone, OffScreenManager::curRenderTargetStatus);
	auto* cList = src_pipeline->Getter_CommandList();

	*paletteMesh->wvpMatrixBuffer[i].matrix.buffMap =
		Get_Orthographic3D(0.0f, CommonV::kWindow_W, 0.0f, CommonV::kWindow_H);

	cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	src_pipeline->SetGraphicsRootSignature();
	src_pipeline->SetPipelineState();

	//VBV
	cList->IASetVertexBuffers(0, 1, paletteMesh->Getter_VertexBufferView());
	//IBV
	cList->IASetIndexBuffer(paletteMesh->Getter_IndexBufferView());

	//texture
	int k = 0;
	for (; k < (int)palette_->WatchUseTexture().size(); ++k)
	{
		cList->SetGraphicsRootDescriptorTable(k,
			shaderBufferData->gpuHandleContainer[palette_->WatchUseTexture()[k]]);
	}

	//Cバッファの場所を指定
	src_pipeline->SetConstantBufferViews(
		k++,
		paletteMesh->wvpMatrixBuffer[i].matrix.GetVirtualGPUAddress());

	auto tmp = palette_->WatchCBufferAddressContainer();

	for (auto itr = tmp.begin();itr != tmp.end(); ++itr, k++)
	{
		cList->SetGraphicsRootConstantBufferView(k,(*itr));
	}

	//描画(DrawCall)。6インデックスで一つのインスタンス
	cList->DrawIndexedInstanced(paletteMesh->kIndexCnt, 1, static_cast<UINT>(usingIndex_index), static_cast<UINT>(usingVertex_index), 0);

	//次の描画用にインクリメント
	paletteMesh->curIndex++;

}

void ExclusiveDraw::DrawMobileQuad(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
	Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
	Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_)
{
	auto* quadMesh = allMesh->Getter_QuadMesh();

	quadMesh->DetectOverDrawing();

	if (drawMode_ == kSolid)
	{
		int i = quadMesh->cur_drawIndex;

		//使用する三角形のマップインデックス
		uint32_t const usingIndex_index = i * quadMesh->indexCnt;
		uint32_t const usingVertex_index = i * quadMesh->vertexCnt;

		Vertex vData[4] =
		{
			leftBottom_,leftTop_,rightBottom_,rightTop_
		};


		uint32_t indices[6] = { 0,1,2,2,1,3 };

		float const i255 = CommonV::inv_255;
		Vector4 color = { color_.x * i255,color_.y * i255,color_.z * i255,color_.w * i255 };

		//< データの入力 >
		//[ インデックス ]
		std::memcpy(&quadMesh->indexMap[usingIndex_index], indices, sizeof(uint32_t) * quadMesh->indexCnt);
		
		//[ 頂点 ]
		std::memcpy(&quadMesh->vertexMap[usingVertex_index], vData, sizeof(Vertex) * quadMesh->vertexCnt);
		
		//[ 行列 ]
		Matrix4 wMat = trans_.GetWorldMatrix();
		Matrix4 wvp = wMat.Multiply(vpMat_);
		*quadMesh->worldMatrixBuffer[i].matrix.buffMap = wMat;
		*quadMesh->wvpMatrixBuffer[i].matrix.buffMap = wvp;

		//[ マテリアル ]
		//色
		Matrix4 uvMat = uvTrans_.GetUVMat();
		quadMesh->materialBuffer[i].material.buffMap->albedoColor = color;
		quadMesh->materialBuffer[i].material.buffMap->uvTransform = uvMat;

		//< データの転送 >
		auto* src_pipeline = allPipelineSet->Getter_pipelineSet(shaderSet_, blendMode_, cullMode_, OffScreenManager::curRenderTargetStatus);
		auto* cList = src_pipeline->Getter_CommandList();

		cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		src_pipeline->SetGraphicsRootSignature();
		src_pipeline->SetPipelineState();

		//VBV
		cList->IASetVertexBuffers(0, 1, quadMesh->Getter_VertexBufferView());

		//IBV
		cList->IASetIndexBuffer(quadMesh->Getter_IndexBufferView());

		//texture
		cList->SetGraphicsRootDescriptorTable(0, shaderBufferData->gpuHandleContainer[texHandle_]);
		//pointLight
		cList->SetGraphicsRootDescriptorTable(1,
			shaderBufferData->gpuHandleContainer[*pLightSrvIndex]);


		//Cバッファの場所を指定
		src_pipeline->SetConstantBufferViews(
			2,
			quadMesh->worldMatrixBuffer[i].matrix.GetVirtualGPUAddress(),
			quadMesh->wvpMatrixBuffer[i].matrix.GetVirtualGPUAddress(),
			quadMesh->materialBuffer[i].material.GetVirtualGPUAddress(),
			dirLightBuffer->dirLight.GetVirtualGPUAddress(),
			cameraParaBuffer->cameraPara.GetVirtualGPUAddress());


		//描画(DrawCall)。6インデックスで一つのインスタンス
		cList->DrawIndexedInstanced(quadMesh->indexCnt, 1, static_cast<UINT>(usingIndex_index), static_cast<UINT>(usingVertex_index), 0);


		//次の描画用にインクリメント
		quadMesh->cur_drawIndex++;
	}
}

void ExclusiveDraw::DrawMobileTriangle(Vertex& left_, Vertex& top_, Vertex& right_,
	Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_ , CullMode cullMode_, int shaderSet_,
	Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_)
{
	if (drawMode_ == kSolid)
	{
		auto* triangleMesh = allMesh->Getter_TriangleMesh();

		triangleMesh->DetectOverDrawing();

		//使用する三角形のマップインデックス
		uint16_t i = triangleMesh->cur_drawIndex;
		uint32_t usingVertex_index = i * triangleMesh->vertexCnt;

		Vertex vData[3] =
		{
			left_,top_,right_
		};

		float const i255 = CommonV::inv_255;
		Vector4 color = { color_.x * i255,color_.y * i255,color_.z * i255,color_.w * i255 };

		//< データのコピー>
		//[ 頂点 ]
		std::memcpy(&triangleMesh->vertexMap[usingVertex_index], vData, sizeof(Vertex) * triangleMesh->vertexCnt);
		
		//[ 行列 ]
		Matrix4 wMat = trans_.GetWorldMatrix();
		Matrix4 wvp = wMat.Multiply(vpMat_);
		*triangleMesh->worldMatrixBuffer[i].matrix.buffMap = wMat;
		*triangleMesh->wvpMatrixBuffer[i].matrix.buffMap = wvp;
		
		//[ マテリアル ]
		//色
		Matrix4 uvMat = uvTrans_.GetUVMat();
		triangleMesh->materialBuffer[i].material.buffMap->albedoColor = color;
		triangleMesh->materialBuffer[i].material.buffMap->uvTransform = uvMat;

		//< データの転送 >
		//< 描画 >
		auto* src_pipeline = allPipelineSet->Getter_pipelineSet(shaderSet_, blendMode_, cullMode_, OffScreenManager::curRenderTargetStatus);
		auto* cList = src_pipeline->Getter_CommandList();

		cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		src_pipeline->SetGraphicsRootSignature();
		src_pipeline->SetPipelineState();

		//VBV
		cList->IASetVertexBuffers(0, 1, triangleMesh->Getter_VertexBufferView());

		cList->SetGraphicsRootDescriptorTable(0, shaderBufferData->gpuHandleContainer[texHandle_]);
		
		//Cバッファの場所を指定
		src_pipeline->SetConstantBufferViews(
			1,
			triangleMesh->worldMatrixBuffer[i].matrix.GetVirtualGPUAddress(),
			triangleMesh->wvpMatrixBuffer[i].matrix.GetVirtualGPUAddress(),
			triangleMesh->materialBuffer[i].material.GetVirtualGPUAddress(),
			dirLightBuffer->dirLight.GetVirtualGPUAddress(),
			cameraParaBuffer->cameraPara.GetVirtualGPUAddress());

		//描画(DrawCall)。3インデックスで一つのインスタンス
		cList->DrawInstanced(triangleMesh->vertexCnt, 1, static_cast<UINT>(usingVertex_index), 0);
		//次の三角形描画用にインクリメント
		triangleMesh->cur_drawIndex++;
	}

}

void ExclusiveDraw::DrawInstancingParticle2D(int numParticles_,Vector4 color_, int texHandle_, 
	BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
	std::vector<Transform> trans_, UVTransform* uvTrans_, Matrix4* vpMat_)
{
	auto* pMesh = allMesh->Getter_TestParticleMesh();
	pMesh->DetectOverDrawing(numParticles_);

	//PSO
	auto* src_pipeline = allPipelineSet->Getter_pipelineSet(shaderSet_, blendMode_, cullMode_, OffScreenManager::curRenderTargetStatus);
	auto* cList = src_pipeline->Getter_CommandList();
	
	cList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	src_pipeline->SetGraphicsRootSignature();
	src_pipeline->SetPipelineState();

	//VBV
	cList->IASetVertexBuffers(0, 1, pMesh->Getter_VertexBufferView());
	//IBV
	cList->IASetIndexBuffer(pMesh->Getter_IndexBufferView());

	//行列データの入力
	for (int i = 0; i < numParticles_; ++i)
	{
		pMesh->transformMatrixStructuredBuffer.matrix.buffMap[i].world = trans_[i].GetWorldMatrix();
		pMesh->transformMatrixStructuredBuffer.matrix.buffMap[i].wvp =
			pMesh->transformMatrixStructuredBuffer.matrix.buffMap[i].world.Multiply(*vpMat_);
	}

	//Material
	float const i255 = CommonV::inv_255;
	Vector4 color = { color_.x * i255,color_.y * i255,color_.z * i255,color_.w * i255 };
	pMesh->materialBuffer.material.buffMap->albedoColor = color;
	pMesh->materialBuffer.material.buffMap->uvTransform = uvTrans_->GetUVMat();

	//トランスフォーム
	cList->SetGraphicsRootDescriptorTable(0, shaderBufferData->gpuHandleContainer[pMesh->srvIndex]);
	//テクスチャ
	cList->SetGraphicsRootDescriptorTable(1, shaderBufferData->gpuHandleContainer[texHandle_]);
	//Cバッファの場所を指定
	cList->SetGraphicsRootConstantBufferView(2, pMesh->materialBuffer.material.GetVirtualGPUAddress());

	//DrawCall
	cList->DrawIndexedInstanced(pMesh->indexCnt, numParticles_, 0, 0,0);
}

