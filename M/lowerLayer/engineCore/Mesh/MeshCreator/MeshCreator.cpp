#include "MeshCreator.h"
#include "./MeshLoader/MeshLoader.h"
#include "../../../../utilities/convertString/convertString.h"
#include "../meshStructure/ModelData/ModelData.h"
#include "../../textureDataManager/textureDataManager.h"
#include "../../Essential/CommandControl/CommandControl.h"
#include "../../Essential/FenceControl/FenceControl.h"
#include "../../../M.h"
#include "../../WinApp.h"


#include <filesystem>
#include <assert.h>

void MeshCreator::Init(AllPipelineSet* allPipelineset_, ID3D12Device* device_, 
	TextureDataManager* textureDataManager_, CommandControl* commandControl_,
	FenceControl* fenceControl_, IDXGISwapChain4* swapChain_)
{
	allPipelineset = allPipelineset_;
	device = device_;
	textureDataManager = textureDataManager_;
	commandControl = commandControl_;
	fenceControl = fenceControl_;
	swapChain = swapChain_;
}

std::unique_ptr<ModelSimple> MeshCreator::CreateModel(std::string filePath_)
{
	std::unique_ptr<ModelSimple> model;
	model = std::make_unique<ModelSimple>(allPipelineset);

	//ファイルからメッシュと、マテリアルデータを読み込む
	std::wstring converted_filePath = ConvertString(filePath_);
	const wchar_t* tmp  = converted_filePath.c_str();

	auto it = loadedModelMap.find(converted_filePath);
	//すでに読み込んである場合はloadedModelMapから取得する
	if (it != loadedModelMap.end())
	{
		model->Getter_ModelData() = it->second->Getter_ModelData();
		*model->GetAppearance() = *it->second->GetAppearance();
		*model->Getter_MeshForModel() = *it->second->Getter_MeshForModel();
		
		int size = (int)model->Getter_ModelData().resMesh.size();
		for (int i = 0; i < size; ++i)
		{
			//メッシュの生成
			model->CreateMesh(device, i);
		}
	}
	// 読み込んでない場合は新たに読み込む
	else
	{
		LoadMesh(tmp, model->Getter_ModelData().resMesh, model->Getter_ModelData().resMaterial);

		//モデルの個数分拡張する
		int size = (int)model->Getter_ModelData().resMesh.size();
		 
		model->ResizeMeshSize(size);

		// コマンドの記録を開始.
		commandControl->PrepareForNextCommandList();

		for (int i = 0; i < size; ++i)
		{
			//メッシュの生成
			model->CreateMesh(device, i);

			//texMapのインデックスの読み込み
			std::filesystem::path fullPath = filePath_;
			std::filesystem::path dirPath = fullPath.parent_path();
			std::string cnv = dirPath.string() + "/";

			auto* resMaterial = model->Getter_ModelDataOfResMaterials(i);
			auto* mesh = model->Getter_MeshForModel(i);

			//カラーマップ
			InputTextureIndex(model.get(), i, Appearance::kColormap, cnv, resMaterial->colorMap);
			//法線マップ
			InputTextureIndex(model.get(), i, Appearance::kNormalmap, cnv, resMaterial->normalMap);
			//スペキュラーマップ
			InputTextureIndex(model.get(), i, Appearance::kSpecularMap, cnv, resMaterial->specularMap);
			//シャインネスマップ
			InputTextureIndex(model.get(), i, Appearance::kShininessMap, cnv, resMaterial->shininessMap);

			mesh->materialBuffer.material.buffMap->diffuse = resMaterial->diffuse;
			mesh->materialBuffer.material.buffMap->shininess = resMaterial->shininess;
			mesh->materialBuffer.material.buffMap->specular = resMaterial->specular;

			//カラーマップがない場合
			if (resMaterial->colorMap.empty())
			{
				(*model->GetAppearance())[i].texHandlesContainer[Appearance::kColormap] =
					M::GetInstance()->GetTexIndex(TextureTag::kWhite2x2);
			}
		}

		for (int i = 0; i < size; ++i)
		{
			auto* resMesh = model->Getter_ModelDataOfResMeshes(i);
			auto* device = WinApp::deviceSetUp.Getter_Device();
			auto* srCreator = WinApp::srvCreator.Getter_StBufferCretaor();

			resMesh->vertexData.CreateAndMapping(device, (UINT)resMesh->vertices.size());
			resMesh->vertexData.gpuHandleIndex = srCreator->CreateSRVForStructuredBuffer((UINT)resMesh->vertices.size(),
				sizeof(Vertex2), resMesh->vertexData.shaderBuffer);
			
			resMesh->uniqueVertexIndexData.CreateAndMapping(device, (UINT)resMesh->uniqueVertexIndices.size());
			resMesh->uniqueVertexIndexData.gpuHandleIndex = srCreator->CreateSRVForStructuredBuffer((UINT)resMesh->uniqueVertexIndices.size(),
				sizeof(uint32_t), resMesh->uniqueVertexIndexData.shaderBuffer);

			resMesh->meshletData.CreateAndMapping(device, (UINT)resMesh->meshlets.size());
			resMesh->meshletData.gpuHandleIndex = srCreator->CreateSRVForStructuredBuffer((UINT)resMesh->meshlets.size(),
				sizeof(ResMeshlet), resMesh->meshletData.shaderBuffer);

			resMesh->primitiveIndicesData.CreateAndMapping(device, (UINT)resMesh->primitiveIndices.size());
			resMesh->primitiveIndicesData.gpuHandleIndex = srCreator->CreateSRVForStructuredBuffer((UINT)resMesh->primitiveIndices.size(),
				sizeof(uint32_t), resMesh->primitiveIndicesData.shaderBuffer);


			//はーBatch
			int k = 0;
			for (auto itr = resMesh->vertices.begin();itr != resMesh->vertices.end();++itr,++k)
			{
				resMesh->vertexData.buffMap[k].position.x = (*itr).position.x;
				resMesh->vertexData.buffMap[k].position.y = (*itr).position.y;
				resMesh->vertexData.buffMap[k].position.z = (*itr).position.z;

				resMesh->vertexData.buffMap[k].normal.x = (*itr).normal.x;
				resMesh->vertexData.buffMap[k].normal.y = (*itr).normal.y;
				resMesh->vertexData.buffMap[k].normal.z = (*itr).normal.z;

				resMesh->vertexData.buffMap[k].tangent.x = (*itr).tangent.x;
				resMesh->vertexData.buffMap[k].tangent.y = (*itr).tangent.y;
				resMesh->vertexData.buffMap[k].tangent.z = (*itr).tangent.z;

				resMesh->vertexData.buffMap[k].texcoord.x = (*itr).texcoord.x;
				resMesh->vertexData.buffMap[k].texcoord.y = (*itr).texcoord.y;

			}
			k = 0;

			for (auto itr = resMesh->uniqueVertexIndices.begin();itr != resMesh->uniqueVertexIndices.end();++itr, ++k)
			{
				resMesh->uniqueVertexIndexData.buffMap[k] = (*itr);
			}
			k = 0;

			for (auto itr = resMesh->meshlets.begin();itr != resMesh->meshlets.end();++itr, ++k)
			{
				resMesh->meshletData.buffMap[k] = (*itr);
			}
			k = 0;

			for (auto itr = resMesh->primitiveIndices.begin();itr != resMesh->primitiveIndices.end();++itr, ++k)
			{
				resMesh->primitiveIndicesData.buffMap[k] = (*itr);
			}
			k = 0;
		}


		commandControl->Getter_commandList()->Close();
		ID3D12CommandList* commandLists[] = { commandControl->Getter_commandList() };
		commandControl->Getter_CommandQueue()->ExecuteCommandLists(1, commandLists);
		fenceControl->WaitFenceEvent(commandControl->Getter_CommandQueue(), swapChain);

		loadedModelMap.emplace(converted_filePath, model.get());		
	}

	return std::move(model);
}

void MeshCreator::InputTextureIndex(ModelSimple* model_,int index_,
	Appearance::TextureType textureHandleIndex_,
	std::string dirPath_,std::wstring textureFile_)
{
	if (textureFile_.empty())
	{
		return;
	}

	//colorMapのインデックスの読み込み
	std::string texMapFilePath = dirPath_ + ConvertString(textureFile_);
	(*model_->GetAppearance())[index_].texHandlesContainer[textureHandleIndex_] =
		textureDataManager->CreateTextureFromFile(texMapFilePath);

}
