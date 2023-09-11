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

	KazMath::Vec3<float> m_cameraEyeDir;
	KazMath::Vec3<float> m_targetOffset;
	float m_cameraEyeDistance;


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
};