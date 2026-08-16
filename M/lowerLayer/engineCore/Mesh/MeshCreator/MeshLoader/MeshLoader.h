#pragma once
#include "../../meshStructure/ModelData/ModelData.h"
#include "../../../external/assimp/include/assimp/Importer.hpp"
#include "../../../external/assimp/include/assimp/scene.h"
#include "../../../external/assimp/include/assimp/postprocess.h"
#include "../../../external/assimp/include/assimp/cimport.h"

class MeshLoader
{
public:
    MeshLoader();
    ~MeshLoader();

    bool Load(
        const wchar_t* filename,
        std::vector<ResMesh>& meshes,
        std::vector<ResMaterial>& materials);

private:
    const aiScene* m_pScene = nullptr;   // シーンデータ.

    void ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh);
    void ParseMaterial(ResMaterial& dstMaterial, const aiMaterial* pSrcMaterial);
    std::wstring Convert(const aiString& path);
    std::string ToUTF8(const std::wstring& value);

};

bool LoadMesh
(
    const wchar_t* filename,
    std::vector<ResMesh>& meshes,
    std::vector<ResMaterial>& materials
);
