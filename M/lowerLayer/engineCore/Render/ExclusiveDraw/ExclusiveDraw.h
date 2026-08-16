#pragma once
#include "../../../../utilities/Transform/Transform.h"
#include "../../../../utilities/UVTransform/UVTransform.h"
#include "../../Mesh/meshStructure/Vertex/Vertex.h"
#include "../../PSO/pipelineCreators/pipelineComponents.h"
#include "../../Mesh/allMesh/MeshForModel/MeshForModelCommon.h"

class AllPipelineSet;
class AllMesh;
struct ShaderBufferData;
struct DirectionalLightBuffer;
struct PointLightBuffer;
struct CameraParaBuffer;
class Palette;

class ExclusiveDraw
{
	AllPipelineSet* allPipelineSet = nullptr;
	AllMesh* allMesh = nullptr;
	CameraParaBuffer* cameraParaBuffer = nullptr;
	uint16_t* pLightSrvIndex = nullptr;
	uint16_t* rLightSrvIndex = nullptr;


public:
	ShaderBufferData* shaderBufferData = nullptr;

	DirectionalLightBuffer* dirLightBuffer = nullptr;

	void Init(AllPipelineSet* allPipelineSet_, AllMesh* allMesh_, 
		ShaderBufferData* shaderBufferData_, CameraParaBuffer* cameraParaBuffer_);

	void DrawMobileTriangle(Vertex& left_, Vertex& top_, Vertex& right_,
		Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_ ,CullMode cullMode_,int shaderSet_,
		Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_);

	void DrawMobileQuad(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
		Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_);
	
	void DrawInstancingParticle2D(int numParticles_, Vector4 color_, int texHandle_,
		BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		std::vector<Transform> trans_, UVTransform* uvTrans_, Matrix4* vpMat_);

	void DrawEllipseWireFrame(Vector3 center_, float radius_, Vector3 rotation_,
		Vector4 color_, Matrix4* vpMat_);
	void DrawLine(Vector3 st_, Vector3 ed_, Vector4 color_, Matrix4* vpMat_);
	void DrawSprite(Vertex& leftTop_, Vertex& rightTop_, Vertex& rightBottom_, Vertex& leftBottom_,
		Vector4 color_, int texHandle_, DrawMode drawMode_, BlendMode blendMode_, CullMode cullMode_, int shaderSet_,
		Transform& trans_, UVTransform& uvTrans_, Matrix4& vpMat_);
	void DrawModel(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_);
	void DrawShaderToy(MeshAndDataCommon* meshAndData_, Matrix4* vpMat_, float time_);

	void ResetDrawIndexes();
	void DrawOnPalette(Palette* palette_);

	void Setter_DirectionalLightBuffer(DirectionalLightBuffer* dirLightBuffer_);
	void Setter_PLightSrvIndex(uint16_t* pLightSrvIndex_);
	void Setter_RLightSrvIndex(uint16_t* rLightSrvIndex_);

};

