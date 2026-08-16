#pragma once

enum DrawMode
{
	kWireFrame,
	kSolid
};

// ブレンドモード
enum BlendMode
{
	kBlendModeNormal = 0, //!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	kBlendModeAdd = 1,    //!< 加算。Src * SrcA + Dest * 1

	kCountOfBlendMode
};

enum CullMode
{
	kCullModeNone,
	kCullModeFront,
	kCullModeBack,
	kCountOfCullMode
};

struct PipelineComponents
{	//[ BlendMode ]
	BlendMode cur_blendMode;
	//[ CullMode ]
	CullMode cur_cullMode;
	//[ ShaderType ]
	int cur_VPshaderSet;

	PipelineComponents();
};


