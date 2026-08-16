#include "MeshLoader.h"
#include <assert.h>
#include <Windows.h>
#include "../../../../../../external/MeshOptimizer/meshoptimizer.h"

bool LoadMesh
(
    const wchar_t* filename,
    std::vector<ResMesh>& meshes,
    std::vector<ResMaterial>& materials
)
{
    MeshLoader loader;
    return loader.Load(filename, meshes, materials);
}


MeshLoader::MeshLoader()
    : m_pScene(nullptr)
{ 
}

MeshLoader::~MeshLoader()
{ 
}

std::string MeshLoader::ToUTF8(const std::wstring& value)
{
    auto length = WideCharToMultiByte(
        CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
    auto buffer = new char[length];

    WideCharToMultiByte(
        CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

    std::string result(buffer);
    delete[] buffer;
    buffer = nullptr;

    return result;
}

std::wstring MeshLoader::Convert(const aiString& path)
{
    wchar_t temp[256] = {};
    size_t  size;
    mbstowcs_s(&size, temp, path.C_Str(), 256);
    return std::wstring(temp);
}

bool MeshLoader::Load
(
    const wchar_t* filename,
    std::vector<ResMesh>& meshes,
    std::vector<ResMaterial>& materials
)
{
    if (filename == nullptr)
    {
        return false;
    }

    // wchar_t から char型(UTF-8)に変換
    auto path = ToUTF8(filename);

    Assimp::Importer importer;
    unsigned int flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;
    flag |= aiProcess_FlipWindingOrder;
    flag |= aiProcess_MakeLeftHanded;
    flag |= aiProcess_FixInfacingNormals;

    // ファイルを読み込み.
    m_pScene = importer.ReadFile(path, flag);

    // チェック.
    if (m_pScene == nullptr)
    {
        return false;
    }

    // メッシュのメモリを確保.
    meshes.clear();
    meshes.resize(m_pScene->mNumMeshes);

    // メッシュデータを変換.
    for (size_t i = 0; i < meshes.size(); ++i)
    {
        const auto pMesh = m_pScene->mMeshes[i];
        ParseMesh(meshes[i], pMesh);
    }

    // マテリアルのメモリを確保.
    materials.clear();
    materials.resize(m_pScene->mNumMaterials);

    // マテリアルデータを変換.
    for (size_t i = 0; i < materials.size(); ++i)
    {
        const auto pMaterial = m_pScene->mMaterials[i];
        ParseMaterial(materials[i], pMaterial);
    }

    // 不要になったのでクリア.
    importer.FreeScene();
    m_pScene = nullptr;

    // 正常終了.
    return true;
}

void MeshLoader::ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh)
{
    // マテリアル番号を設定
    dstMesh.materialId = pSrcMesh->mMaterialIndex;

    aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // 頂点データのメモリを確保
    dstMesh.vertices.resize(pSrcMesh->mNumVertices);

    for (auto i = 0u; i < pSrcMesh->mNumVertices; ++i)
    {
        auto pPosition = &(pSrcMesh->mVertices[i]);
        auto pNormal = &(pSrcMesh->mNormals[i]);
        auto pTexCoord = (pSrcMesh->HasTextureCoords(0)) ? &(pSrcMesh->mTextureCoords[0][i]) : &zero3D;
        auto pTangent = (pSrcMesh->HasTangentsAndBitangents()) ? &(pSrcMesh->mTangents[i]) : &zero3D;

        dstMesh.vertices[i] = Vertex(
            Vector3(pPosition->x, pPosition->y, pPosition->z),
            Vector2(pTexCoord->x, 1.0f - pTexCoord->y),
            Vector3(pNormal->x, pNormal->y, pNormal->z),
            Vector3(pTangent->x, pTangent->y, pTangent->z)
        );
    }


    // 頂点インデックスのメモリを確保
    dstMesh.indices.resize(pSrcMesh->mNumFaces * 3);

    for (auto i = 0u; i < pSrcMesh->mNumFaces; ++i)
    {
        const auto& face = pSrcMesh->mFaces[i];
        assert(face.mNumIndices == 3);

        dstMesh.indices[i * 3 + 0] = face.mIndices[0];
        dstMesh.indices[i * 3 + 1] = face.mIndices[1];
        dstMesh.indices[i * 3 + 2] = face.mIndices[2];
    }

    //最適化
    {
        std::vector<uint32_t> remap(dstMesh.indices.size());

        //重複データを再マップするための再マッピング用インデックスを生成
        auto vertexCnt = meshopt_generateVertexRemap(
            remap.data(),
            dstMesh.indices.data(),
            dstMesh.indices.size(),
            dstMesh.vertices.data(),
            dstMesh.vertices.size(),
            sizeof(Vertex)
        );

        std::vector<Vertex> vertices(vertexCnt);
        std::vector<uint32_t> indices(dstMesh.indices.size());

        //頂点インデックスを再マッピング
        meshopt_remapIndexBuffer(
            indices.data(),
            dstMesh.indices.data(),
            dstMesh.indices.size(),
            remap.data()
        );

        //頂点データを再マッピング
        meshopt_remapVertexBuffer(
            vertices.data(),
            dstMesh.vertices.data(),
            dstMesh.vertices.size(),
            sizeof(Vertex),
            remap.data());

        //不要になったメモリを解放
        remap.clear();
        remap.shrink_to_fit();

        //最適化したサイズにメモリ量を減らす
        dstMesh.vertices.resize(vertices.size());
        dstMesh.indices.resize(indices.size());

        //頂点キャッシュ最適化
        meshopt_optimizeVertexCache(
            dstMesh.indices.data(),
            indices.data(),
            indices.size(),
            vertexCnt);

        //不要になったメモリを解放
        indices.clear();
        indices.shrink_to_fit();

        //頂点フェッチ最適化
        meshopt_optimizeVertexFetch(
            dstMesh.vertices.data(),
            dstMesh.indices.data(),
            dstMesh.indices.size(),
            vertices.data(),
            vertices.size(),
            sizeof(Vertex)
        );

        //不要になったメモリを解放
        vertices.clear();
        vertices.shrink_to_fit();

    }

    //メッシュレット生成
        // ステージ 3: メッシュレットの自動分割と10bitパッキング
    {
        // 3-1. ハードウェア（Mesh Shader）の制約条件。
        // 一般的な現代のGPUアーキテクチャに最適化するため、
        // 1つのメッシュレットに含まれる最大頂点数を「64」、最大三角形数を「124〜126」に設定
        const size_t kMaxVertices = 64;
        const size_t kMaxPrimitives = 126;
        const float  kConeWeight = 0.0f; // カリング用のコーン計算の重み（今回は不使用なので0）

        // 3-2. 分割後に生成されるメッシュレットの「理論上の最大上限数」をあらかじめ計算
        size_t max_meshlets = meshopt_buildMeshletsBound(dstMesh.indices.size(), kMaxVertices, kMaxPrimitives);

        // 3-3. 最新APIが必要とするテンポラリ出力バッファを確保
        // 最新仕様では、メタデータ、頂点リマップ、ローカル三角形トポロジーを別々のフラット配列に書き出します
        std::vector<meshopt_Meshlet> temp_meshlets(max_meshlets);
        std::vector<unsigned int>    temp_meshlet_vertices(max_meshlets * kMaxVertices);
        std::vector<unsigned char>   temp_meshlet_triangles(max_meshlets * kMaxPrimitives * 3);

        // 3-4. 【最新APIによるメッシュレット分割の実行】
        // 最適化済みの頂点バッファ(vertices)から、座標データの先頭ポインタとストライド(バイトサイズ)を直接渡すことで、
        // 空間的な局所性を考慮した非常に高度なクラスター分割アルゴリズムが実行されます。
        size_t meshlet_count = meshopt_buildMeshlets(
            temp_meshlets.data(),
            temp_meshlet_vertices.data(),
            temp_meshlet_triangles.data(),
            dstMesh.indices.data(),
            dstMesh.indices.size(),
            &dstMesh.vertices[0].position.x, // 頂点データの座標ポインタの先頭
            dstMesh.vertices.size(),
            sizeof(Vertex),              // 頂点構造体1つあたりのバイトサイズ
            kMaxVertices,
            kMaxPrimitives,
            kConeWeight
        );

        // 実際に生成された確定数に合わせてメタデータ配列をリサイズ
        temp_meshlets.resize(meshlet_count);


        // 3-5. 出力先（dstMesh）の各種コンテナのメモリ領域をがっちり確保
        // 最後のメッシュレットのオフセットを参照することで、実際に使われた正確な要素数が割り出せるため、
        // 無駄のない完璧なサイズで一元化配列のメモリを予約（reserve）できます。
        const meshopt_Meshlet& last_meshlet = temp_meshlets.back();
        size_t total_unique_vertices = last_meshlet.vertex_offset + last_meshlet.vertex_count;
        size_t total_primitive_triangles = last_meshlet.triangle_offset + last_meshlet.triangle_count;

        dstMesh.meshlets.resize(meshlet_count);
        dstMesh.uniqueVertexIndices.reserve(total_unique_vertices);
        dstMesh.primitiveIndices.reserve(total_primitive_triangles);


        // 3-6. 生成されたデータをエンジン独自のデータ構造（ResMesh）へ詰め替える
        for (size_t i = 0; i < meshlet_count; ++i)
        {
            const meshopt_Meshlet& src = temp_meshlets[i];
            ResMeshlet& dest = dstMesh.meshlets[i];

            // --- A. 管理用メタデータ（オフセット・カウント）の設定 ---
            // シェーダー側が「巨大な統合バッファのどこから何個読めばいいか」を正確に指し示すための情報を記録します
            dest.vertexOffset = static_cast<uint32_t>(dstMesh.uniqueVertexIndices.size());
            dest.vertexCnt = src.vertex_count;
            dest.primitiveOffset = static_cast<uint32_t>(dstMesh.primitiveIndices.size());
            dest.primitiveCnt = src.triangle_count;

            // --- B. ユニーク頂点インデックスの格納 ---
            // 最新仕様のフラット配列「temp_meshlet_vertices」から、このメッシュレットが使用する範囲（src.vertex_offset から数個分）
            // を切り出して、エンジンのグローバル配列へと追記（プッシュバック）していきます
            for (unsigned int v = 0; v < src.vertex_count; ++v)
            {
                uint32_t global_vertex_idx = temp_meshlet_vertices[src.vertex_offset + v];
                dstMesh.uniqueVertexIndices.push_back(global_vertex_idx);
            }

            // --- C. ローカル三角形インデックスのパッキング（10bit×3マス） ---
            // 「temp_meshlet_triangles」には、1つのメッシュレット内（0〜63番）のローカル頂点番号が1バイト（unsigned char）型で入っています。
            // これを元のワインディング順序反転（1 -> 0 -> 2）を維持したまま、1つの32bit型ビットフィールド『ResPrimitiveIndex』にパックして格納します
            for (unsigned int t = 0; t < src.triangle_count; ++t)
            {
                size_t prim_base_idx = src.triangle_offset + t * 3;

                ResPrimitiveIndex tris = {};
                
                tris.index2 = temp_meshlet_triangles[prim_base_idx + 0];
                tris.index0 = temp_meshlet_triangles[prim_base_idx + 1];
                tris.index1 = temp_meshlet_triangles[prim_base_idx + 2];
                tris.reserved = 0; 

                dstMesh.primitiveIndices.push_back(tris);
            }
        }

        //最終メモリの最適化
        dstMesh.uniqueVertexIndices.shrink_to_fit();
        dstMesh.primitiveIndices.shrink_to_fit();
        dstMesh.meshlets.shrink_to_fit();

    }
}

void MeshLoader::ParseMaterial(ResMaterial& dstMaterial, const aiMaterial* pSrcMaterial)
{
    // 拡散反射成分
    {
        aiColor3D color(0.0f, 0.0f, 0.0f);

        if (pSrcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            dstMaterial.diffuse.x = color.r;
            dstMaterial.diffuse.y = color.g;
            dstMaterial.diffuse.z = color.b;
        }
        else
        {
            dstMaterial.diffuse.x = 0.5f;
            dstMaterial.diffuse.y = 0.5f;
            dstMaterial.diffuse.z = 0.5f;
        }
    }

    // 鏡面反射成分
    {
        aiColor3D color(0.0f, 0.0f, 0.0f);

        if (pSrcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        {
            dstMaterial.specular = { color.r,color.g,color.b };
        }
        else
        {
            dstMaterial.specular = { 0.0f,0.0f,0.0f };
        }
    }

    // 鏡面反射強度
    {
        auto shininess = 0.0f;
        if (pSrcMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        {
            dstMaterial.shininess = shininess;
        }
        else
        {
            dstMaterial.shininess = 0.0f;
        }
    }

    // colorマップ
    {
        aiString path;
        if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
        {
            dstMaterial.colorMap = Convert(path);
        }
        else
        {
            dstMaterial.colorMap.clear();
        }
    }

    // スペキュラーマップ
    {
        aiString path;
        if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS)
        {
            dstMaterial.specularMap = Convert(path);
        }
        else
        {
            dstMaterial.specularMap.clear();
        }
    }

    // シャイネスマップ
    {
        aiString path;
        
        if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS)
        {
            
            dstMaterial.shininessMap = Convert(path);
        }
        else
        {
            dstMaterial.shininessMap.clear();
        }
    }

    // 法線マップ
    {
        aiString path;
        if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_NORMALS(0), path) == AI_SUCCESS)
        {
            dstMaterial.normalMap = Convert(path);
        }
        else
        {
            if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_HEIGHT(0), path) == AI_SUCCESS)
            {
                dstMaterial.normalMap = Convert(path);
            }
            else
            {
                dstMaterial.normalMap.clear();
            }
        }
    }
}
