#pragma once
#include "../../OriginalScreen/OriginalScreen.h"
#include "../../../Buffer/constantBuffer/SimpleNeonLikeBuffer/SimpleNeonLikeBuffer.h"
#include "../../../Buffer/constantBuffer/GaussianBlurBuffer/GaussianBlurBuffer.h"
#include <memory>

class PostEffectBuffer;
class TextureDataManager;
class DOF_Screen;

namespace PostEffects
{
	struct NoEffection :public OffScreen
	{
		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		NoEffection(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_);

	};

	struct GreyScale :public OffScreen
	{
		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		GreyScale(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_);
	};

	class SimpleNeonLike :public OffScreen
	{
		SimpleNeonLikePara para;
		SimpleNeonLikeBuffer cBuffer;

	public:
		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		SimpleNeonLike(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_);
	};

	struct Brightness :public OffScreen
	{
		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;

		Brightness(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_);
	};

	struct SideBlur :public OffScreen
	{
		GaussianWeight gaussianParam;
		GaussianBlurBuffer gaussianBlurparaBuffer;

		Brightness* brightness;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		SideBlur(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_, Brightness* brightness_);
	};

	struct VerticalBlur :public OffScreen
	{
		GaussianWeight gaussianParam;
		GaussianBlurBuffer gaussianBlurparaBuffer;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		VerticalBlur(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_);
	};

	struct Half :public OffScreen
	{
		GaussianWeight gaussianParam;
		GaussianBlurBuffer gaussianBlurparaBuffer;

		VerticalBlur* verticalBlur;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		Half(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_, VerticalBlur* verticalBlur_);
	};

	struct Quater :public OffScreen
	{
		GaussianWeight gaussianParam;
		GaussianBlurBuffer gaussianBlurparaBuffer;

		Half* half;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		Quater(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_, Half* half_);
	};

	struct GaussianBlur :public OffScreen
	{
		Brightness* brightness;
		SideBlur* sideBlur;
		VerticalBlur* verticalBlur;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		
		GaussianBlur(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,
			SideBlur* sideBlur_, VerticalBlur* verticalBlur_, Brightness* brightness_);

	};

	struct Bloom :public OffScreen
	{
		Brightness* brightness;
		SideBlur* sideBlur;
		VerticalBlur* verticalBlur;
		Half* half;
		Quater* quater;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		Bloom(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,
			Brightness* brightness_, SideBlur* sideBlur_, VerticalBlur* verticalBlur_, Half* half_, Quater* quater_);
	};

	struct DepthOfField :public OffScreen
	{
		DOF_Screen* dOF_Screen;
		GaussianBlur* gaussianBlur;

		virtual void Update()override;
		virtual void Init(ID3D12Device* device_) override;
		DepthOfField(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,
			DOF_Screen* dOF_Screen_, GaussianBlur* gaussianBlur_);

	};

	

}
