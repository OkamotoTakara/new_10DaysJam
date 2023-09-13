#pragma once
#include"../KazLibrary/Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/DrawFunc/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../Game/Render/GameRender.h"
#include"../Game/Render/BasicDraw.h"
#include"../Game/DrawCallSet.h"
#include"../Game/DrawCallUISet.h"

class Player;
class MineralMgr;
class MineralTarget;
class Core;
class EnemyMgr;

class GameScene :public SceneBase
{

private:

	std::shared_ptr<Player> m_player;
	std::shared_ptr<MineralTarget> m_mineralTarget;
	std::shared_ptr<Core> m_goldCore;
	std::shared_ptr<EnemyMgr> m_enemyMgr;
	DebugCamera m_camera;

	KazMath::Transform3D m_stageTransform;

	DrawCallSet m_ground;
	DrawCallSet m_fence;
	DrawCallSet m_rock;
	DrawCallSet m_tree;
	DrawCallSet m_line;

	DrawCallUISet m_titleLogoUI;
	DrawCallUISet m_titleBackGroundUI;
	DrawCallUISet m_titleStartUI;
	DrawCallUISet m_titleQuitUI;

	DrawCallUISet m_resultBackGroundUI;
	DrawCallUISet m_resultTitleUI;
	DrawCallUISet m_resultRetryUI;

	KazMath::Vec3<float> m_cameraEyeDir;
	KazMath::Vec3<float> m_targetOffset;
	float m_cameraEyeDistance;

	//�^�C�g���֘A
	int m_selectTitleNum;
	float m_selectTitleUISine;
	const float ADD_SELECT_TITLE_SINE = 0.2f;
	const float SELECT_TITLE_SINE_SCALE = 15.0f;

	//�^�C�g���Ń��S�Ƃ��̃A���t�@�������܂ł̃^�C�}�[
	float m_titleLogoDeleteTimer;
	const float TITLELOGO_DELETE_TIMER = 10.0f;

	//���U���g�֘A�B
	int m_selectResultNum;
	bool m_isResultToTitle;
	bool m_isResultToGame;

	//UI�̃X�P�[��
	KazMath::Vec2<float> UI_DEF_START_SCALE = KazMath::Vec2<float>(150.0f, 50.0f);
	KazMath::Vec2<float> UI_DEF_QUIT_SCALE = KazMath::Vec2<float>(125.0f, 50.0f);
	KazMath::Vec2<float> UI_MAX_START_SCALE = UI_DEF_START_SCALE * 1.5f;
	KazMath::Vec2<float> UI_MAX_QUIT_SCALE = UI_DEF_QUIT_SCALE * 1.5f;

	KazMath::Vec2<float> UI_DEF_TITLE_SCALE = KazMath::Vec2<float>(100.0f, 50.0f);
	KazMath::Vec2<float> UI_DEF_RETRY_SCALE = KazMath::Vec2<float>(100.0f, 50.0f);
	KazMath::Vec2<float> UI_MAX_TITLE_SCALE = KazMath::Vec2<float>(100.0f, 50.0f) * 1.3f;
	KazMath::Vec2<float> UI_MAX_RETRY_SCALE = KazMath::Vec2<float>(100.0f, 50.0f) * 1.3f;


public:
	GameScene();
	~GameScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:

	void UpdateTitle();
	void UpdateResult();
};