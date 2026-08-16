#pragma once
#include <unordered_map>
#include <string>


class TextureDataManager;

enum class TextureTag
{
	kWhite2x2,
	kSouhei,
	kPlayer,
	kNothing,
	kGridN,
	kAtlasNumbers,
	kOh,
	kCursorImage,
	kNormalDekoboko,
	kEnemyN,
	kEnemyBodyN,
	kEnemyThornN,
	kAllyN,

	// 汎用
	kNumbers1000x100,		// numbers
	kCursor50x50,			// cursor

	// 操作説明UI
	kButtonBack200x60,		// モドル
	kButtonDecision200x60,	// ケッテイ
	kButtonMove200x60,		// イドウ
	kButtonOption200x60,	// セッテイ
	kButtonPause200x60,		// ポーズ
	kButtonReticle200x60,	// ネラウ
	kButtonSet200x60,		// ハイチ
	kButtonShot200x60,		// ショット
	kButtonZoom200x60,		// ズーム


	// ポーズUI
	Pause500x100, 			// ポーズタイトル
	PauseButton01_350x50,	// プレイ
	PauseButton02_350x50,	// リトライ
	PauseButton03_350x50,	// オプション
	PauseButton04_350x50,	// セレクト
	PauseScreen_661x720,	// ポーズ背景

	// インゲームUI
	IngameButton200x300,	// 全部まとめたやつ
	kGameOver1000x200,		// ゲームオーバー
	kGameClear1000x200,		// ゲームクリア

	// ステージセレクトUI
	kStageName01_1000x100,
	kStageName02_1000x100,
	kStageName03_1000x100,
	kStageName04_1000x100,
	kStageName05_1000x100,
	kArrowLeft100x200,
	kArrowRight100x200,

	// リザルトUI
	GameOver1000x200,
	GameClear1000x200,

	kGroundN,

	kPlayerHeadN,
	kPlayerHandN,
	kPlayerLegN,
	kPlayerBodyN,
	kCannonN,

	kTowerN,
	kFactoryN,

	kBulletN,

	kScrewN,

	kCirclen,
	kCirclenN,

	kPushA,

	kCount
};

class ResourceContainer
{
	std::unordered_map<TextureTag, int> map_textureID;
	void LoadTextureFile(TextureTag tag_, std::string filePath_);
	void LoadAllTextureFile();
	TextureDataManager* textureManager = nullptr;

public:

	void Init(TextureDataManager* textureManager_);
	int GetTextureIndex(TextureTag tag_);

};

