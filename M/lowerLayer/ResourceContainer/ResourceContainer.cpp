#include "ResourceContainer.h"
#include "../engineCore/textureDataManager/textureDataManager.h"
#include "../engineCore/WinApp.h"

void ResourceContainer::LoadAllTextureFile()
{
	std::string pathToPresetTex = "./resource/preset/texture/";

	LoadTextureFile(TextureTag::kWhite2x2, pathToPresetTex + "white2x2.png");
	LoadTextureFile(TextureTag::kSouhei, pathToPresetTex + "gradationLine.png");
	LoadTextureFile(TextureTag::kPlayer, pathToPresetTex + "player128x128.png");
	LoadTextureFile(TextureTag::kNothing, pathToPresetTex + "header.jpg");
	LoadTextureFile(TextureTag::kGridN, pathToPresetTex + "gridLineNormal.png");
	LoadTextureFile(TextureTag::kAtlasNumbers, pathToPresetTex + "numbers48x48.png");
	LoadTextureFile(TextureTag::kOh, pathToPresetTex + "oh.png");
	LoadTextureFile(TextureTag::kCursorImage, pathToPresetTex + "cursorImage.png");
	LoadTextureFile(TextureTag::kNormalDekoboko, pathToPresetTex + "dekoboko.png");
	LoadTextureFile(TextureTag::kKadai, pathToPresetTex + "kadai.dds");

	
}

int ResourceContainer::GetTextureIndex(TextureTag tag_)
{
	return map_textureID[tag_];
}

void ResourceContainer::Init(TextureDataManager* textureManager_)
{
	static bool onlyOnce = true;
	if (onlyOnce)
	{
		textureManager = textureManager_;
		LoadAllTextureFile();
		Log(WinApp::log, "Complete : LoadTexture");

		onlyOnce = false;
	}

}

void ResourceContainer::LoadTextureFile(TextureTag tag_, std::string filePath_)
{
	map_textureID[tag_] = textureManager->CreateTextureFromFile(filePath_);
}