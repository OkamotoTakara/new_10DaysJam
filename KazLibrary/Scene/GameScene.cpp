#include "GameScene.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"
#include"Imgui/MyImgui.h"
#include"Loader/ObjResourceMgr.h"
#include"Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"Math/KazMath.h"
#include"../Game/Player.h"
#include"../Game/Effect/ShakeMgr.h"
#include"../Game/Mineral/MineralMgr.h"
#include"../Game/Rock/RockMgr.h"
#include"../Game/Mineral/MineralTarget.h"
#include"../Game/Core/Core.h"
#include"../Game/Enemy/EnemyMgr.h"
#include"../Game/DestructibleObject/DestructibleObjectMgr.h"
#include"../Game/BuildingMaterial/BuildingMaterialMgr.h"
#include"../Game/Building/BuildingMgr.h"
#include"../Game/UI/NumberFont.h"
#include"../Game/Collision/StageCollision.h"
#include"../Game/Enemy/EnemyRoute.h"
#include"../Game/WallAndTreeGeneratePos.h"
#include"../Game/Wave/WaveMgr.h"
#include"../Game/TitleFlag.h"
#include"../Game/Tutorial.h"
#include"../KazLibrary/Easing/easing.h"
#include"../Game/Transition.h"

GameScene::GameScene()
{
	skipTurtorialFlag = false;
	PreInit();
	firstGenerateFlag = true;

	m_stageTransform.scale = { 5.0f, 5.0f, 5.0f };
	m_stageTransform.pos = { 0.0f, -5.0f, 0.0f };
	m_stageTransform.rotation = { 0.0f, 197.5f, 0.0f };
	StageCollision::Instance()->Setting("Resource/Stage/Collision/", "StageCollision.gltf", m_stageTransform);
	m_cameraEyeDistance = 1200.0f;


	const float CAMERA_EYE_X = -400.0f;
	const float CAMERA_EYE_Y = 600.0f;
	const float CAMERA_EYE_Z = -300.0f;
	m_cameraEyeDir = { CAMERA_EYE_X, CAMERA_EYE_Y, CAMERA_EYE_Z };
	m_cameraEyeDir = { -0.691f, 0.503f, -0.519f };
	m_cameraEyeDir.Normalize();

	m_player = std::make_unique<Player>();

	m_mineralTarget = std::make_shared<MineralTarget>();
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_goldCore = std::make_shared<Core>();
	m_goldCore->SetPos({ 169.0f,10.0f,105.0f });
	MineralMgr::Instance()->Setting();
	RockMgr::Instance()->Setting();
	DestructibleObjectMgr::Instance()->Setting();
	BuildingMaterialMgr::Instance()->Setting();
	BuildingMgr::Instance()->Setting();
	EnemyRoute::Instance()->Setting();
	WallAndTreeGeneratePos::Instance()->Setting();
	WaveMgr::Instance()->Setting();
	Transition::Instance()->Setting();


	m_ground.Load("Resource/Stage/", "Stage_Ground.gltf");
	m_fence.Load("Resource/Stage/", "Stage_Fence.gltf");
	m_tree.Load("Resource/Stage/", "Stage_Tree.gltf");
	m_rock.Load("Resource/Stage/", "Stage_Rock.gltf");

	//タイトルロゴをロード
	m_titleLogoUI.Load("Resource/Title/TitleLogo.png");
	m_titleBackGroundUI.Load("Resource/Title/Title_1.png");
	m_titleStartUI.Load("Resource/Title/Title_2.png");
	m_titleQuitUI.Load("Resource/Title/Title_3.png");

	//タイトルロゴの場所を設定。
	m_titleLogoUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_titleLogoUI.m_transform.scale = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_titleBackGroundUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_titleBackGroundUI.m_transform.scale = { 1280.0f, 720.0f };
	m_titleStartUI.m_transform.pos = { 640.0f, 440.0f };
	m_titleStartUI.m_transform.scale = UI_MAX_START_SCALE;
	m_titleQuitUI.m_transform.pos = { 640.0f, 545.0f };
	m_titleQuitUI.m_transform.scale = { UI_DEF_QUIT_SCALE };
	m_selectTitleNum = 0;
	m_selectTitleUISine = 0.0f;

	NumberFont::Instance()->Load();
	Tutorial::Instance()->setting();

	Tutorial::Instance()->is_tutorial = false;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	skipTurtorialFlag = false;

	m_player->Init();
	MineralMgr::Instance()->Init();
	//MineralMgr::Instance()->DebugGenerate();
	m_goldCore->Init();

	m_enemyMgr->Init();
	//m_enemyMgr->DebugGenerate();

	RockMgr::Instance()->Init();
	//m_rockMgr->DebugGenerate();

	DestructibleObjectMgr::Instance()->Init();
	//m_destructibleObject->DebugGenerate();

	WaveMgr::Instance()->Init(m_enemyMgr);
	WaveMgr::Instance()->GameStart();

	BuildingMaterialMgr::Instance()->Init();
	BuildingMgr::Instance()->Init();
	BuildingMgr::Instance()->Generate();

	//太陽の方向を昼に設定
	GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir = KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f);

	TitleFlag::Instance()->Init();
	m_titleLogoDeleteTimer = 0.0f;

	Transition::Instance()->Init();

	m_titleStartUI.m_color.color.a = 255;
	m_titleQuitUI.m_color.color.a = 255;
	m_titleBackGroundUI.m_color.color.a = 255;

}

void GameScene::PreInit()
{
}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
}

void GameScene::Update()
{
	using namespace KazMath;

	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_R)) {
		Init();
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_I)) {
		TitleFlag::Instance()->m_isTitle = false;
	}

	//ウェーブを更新。
	WaveMgr::Instance()->Update(m_enemyMgr);

	//プレイヤーを更新。
	m_player->Update();

	//ミネラルを更新。
	MineralMgr::Instance()->Update(m_player, m_mineralTarget);

	//岩を更新。
	RockMgr::Instance()->Update(m_player);

	//敵を更新。
	m_enemyMgr->Update(m_goldCore, m_player);

	//ミネラルのターゲットを更新。
	m_mineralTarget->Update(m_player->GetPosZeroY(), m_player->GetMineralAffectRange(), m_enemyMgr);

	//黄金のコアを更新
	m_goldCore->Update();

	//破壊可能オブジェクトを更新。
	DestructibleObjectMgr::Instance()->Update();

	//建築物の素材を更新。
	BuildingMaterialMgr::Instance()->Update();

	//建築物を更新。
	BuildingMgr::Instance()->Update(m_player);

	//カメラの位置を設定
	m_cameraEyeDir.Normalize();
	const float CAMERA_TARGET_X = 0.0f;
	const float CAMERA_TARGET_Y = 5.0f;
	const float CAMERA_TARGET_Z = 10.0f;
	CameraMgr::Instance()->Camera(m_player->GetTransformDefaultY() + m_cameraEyeDir * m_cameraEyeDistance, m_player->GetTransformDefaultY() + Vec3<float>(CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z) + ShakeMgr::Instance()->GetShakeVec3(), Vec3<float>(0, 1, 0));

	//シェイクを更新。
	ShakeMgr::Instance()->Update();

	//タイトル画面だったら。
	if (TitleFlag::Instance()->m_isTitle) {


		if (KeyBoradInputManager::Instance()->InputState(DIK_W) ||
			KeyBoradInputManager::Instance()->InputState(DIK_UP) ||
			ControllerInputManager::Instance()->InputStickTrigger(ControllerStickSide::LEFT_STICK, ControllerSide::UP_SIDE)) {

			m_selectTitleNum = std::clamp(m_selectTitleNum - 1, 0, 1);

		}

		if (KeyBoradInputManager::Instance()->InputState(DIK_S) ||
			KeyBoradInputManager::Instance()->InputState(DIK_DOWN) ||
			ControllerInputManager::Instance()->InputStickTrigger(ControllerStickSide::LEFT_STICK, ControllerSide::DOWN_SIDE)) {

			m_selectTitleNum = std::clamp(m_selectTitleNum + 1, 0, 1);

		}

		//決定キーが入力されたら
		if (KeyBoradInputManager::Instance()->InputState(DIK_SPACE) ||
			KeyBoradInputManager::Instance()->InputState(DIK_RETURN) ||
			ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_A)) {

			//Startを選択していたら。
			if (m_selectTitleNum == 0) {

				TitleFlag::Instance()->m_isStart = true;

			}
			//Quitを選択していたら
			else if (m_selectTitleNum) {

				TitleFlag::Instance()->m_isQuit = true;

			}

		}


		m_selectTitleUISine += ADD_SELECT_TITLE_SINE;

	}

	Tutorial::Instance()->Update();

	//タイトルロゴを消すまでのタイマー
	if (TitleFlag::Instance()->m_isStart && TitleFlag::Instance()->m_isDrawTitle) {

		m_titleLogoDeleteTimer = std::clamp(m_titleLogoDeleteTimer + 1.0f, 0.0f, TITLELOGO_DELETE_TIMER);

		//イージングをかけてタイマーを減らす。
		float easingAmount = EasingMaker(In, Sine, m_titleLogoDeleteTimer / TITLELOGO_DELETE_TIMER);

		//アルファを減らす。
		m_titleStartUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		m_titleQuitUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		m_titleBackGroundUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);

		if (TITLELOGO_DELETE_TIMER <= m_titleLogoDeleteTimer) {

			m_titleLogoDeleteTimer = 0.0f;
			TitleFlag::Instance()->m_isDrawTitle = false;
			Transition::Instance()->Activate();

		}

	}

	//タイトルが消えていて、遷移も終わっていたらタイトルを終える。
	if (!TitleFlag::Instance()->m_isDrawTitle && !Transition::Instance()->GetIsActive()) {

		TitleFlag::Instance()->m_isTitle = false;

	}

	Transition::Instance()->Update();

}

void GameScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//UIを表示
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "UI");


	WaveMgr::Instance()->Draw(arg_rasterize, arg_blasVec);


	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

	m_player->Draw(arg_rasterize, arg_blasVec);
	MineralMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	RockMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	m_mineralTarget->Draw(arg_rasterize, arg_blasVec, m_enemyMgr);
	m_enemyMgr->Draw(arg_rasterize, arg_blasVec);
	m_goldCore->Draw(arg_rasterize, arg_blasVec);
	DestructibleObjectMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	BuildingMaterialMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	BuildingMgr::Instance()->Draw(arg_rasterize, arg_blasVec);

	m_ground.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_fence.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_tree.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_rock.Draw(arg_rasterize, arg_blasVec, m_stageTransform);


	ImGui::Begin("UI");

	//ImGui::DragFloat("POS_X", &BuildingMgr::Instance()->GetWall(2).lock()->m_initPos.x, 1.0f);
	//ImGui::DragFloat("POS_Y", &BuildingMgr::Instance()->GetWall(2).lock()->m_initPos.y, 1.0f);
	//ImGui::DragFloat("POS_Z", &BuildingMgr::Instance()->GetWall(2).lock()->m_initPos.z, 1.0f);
	//ImGui::DragFloat("SCALE_X", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.scale.x, 0.01f);
	//ImGui::DragFloat("SCALE_Y", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.scale.y, 0.01f);
	//ImGui::DragFloat("SCALE_Z", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.scale.z, 0.01f);
	//ImGui::DragFloat("ROTATE_X", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.rotation.x, 0.1f);
	//ImGui::DragFloat("ROTATE_Y", &BuildingMgr::Instance()->GetWall(2).lock()->m_rotateY, 0.1f);
	//ImGui::DragFloat("ROTATE_Z", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.rotation.z, 0.1f);

	//ImGui::Text(" ");

	//ImGui::DragFloat("EyeDistance", &m_cameraEyeDistance, 1.0f);
	ImGui::DragFloat("START_UI_X", &m_titleStartUI.m_transform.pos.x, 1.0f);
	ImGui::DragFloat("START_UI_Y", &m_titleStartUI.m_transform.pos.y, 1.0f);
	ImGui::DragFloat("QUIT_UI_X", &m_titleQuitUI.m_transform.pos.x, 1.0f);
	ImGui::DragFloat("QUIT_UI_Y", &m_titleQuitUI.m_transform.pos.y, 1.0f);
	//ImGui::SliderFloat("UI_Z", &GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.z, 0.0f, 1.0f);

	ImGui::End();

	//タイトルのUIをロード
	if (TitleFlag::Instance()->m_isTitle) {

		//Startを選択していたら。
		if (m_selectTitleNum == 0) {
			m_titleStartUI.m_transform.scale += ((UI_MAX_START_SCALE + sinf(m_selectTitleUISine) * SELECT_TITLE_SINE_SCALE) - m_titleStartUI.m_transform.scale) / 3.0f;
			m_titleQuitUI.m_transform.scale += (UI_DEF_QUIT_SCALE - m_titleQuitUI.m_transform.scale) / 3.0f;
		}
		else {
			m_titleStartUI.m_transform.scale += (UI_DEF_START_SCALE - m_titleStartUI.m_transform.scale) / 3.0f;
			m_titleQuitUI.m_transform.scale += ((UI_MAX_QUIT_SCALE + sinf(m_selectTitleUISine) * SELECT_TITLE_SINE_SCALE) - m_titleQuitUI.m_transform.scale) / 3.0f;
		}


	}
	if (TitleFlag::Instance()->m_isDrawTitle) {
		m_titleStartUI.Draw(arg_rasterize);
		m_titleQuitUI.Draw(arg_rasterize);
		m_titleBackGroundUI.Draw(arg_rasterize);
	}

	//チュートリアルを描画。
	Tutorial::Instance()->Draw(arg_rasterize, arg_blasVec);

	//遷移を描画。
	Transition::Instance()->Draw(arg_rasterize, arg_blasVec);

}

int GameScene::SceneChange()
{
	return SCENE_NONE;
}
