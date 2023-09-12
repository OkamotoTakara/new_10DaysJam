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
	WaveMgr::Instance()->GameStart();


	m_ground.Load("Resource/Stage/", "Stage_Ground.gltf");
	m_fence.Load("Resource/Stage/", "Stage_Fence.gltf");
	m_tree.Load("Resource/Stage/", "Stage_Tree.gltf");
	m_rock.Load("Resource/Stage/", "Stage_Rock.gltf");

	NumberFont::Instance()->Load();
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

	//DessolveOutline outline;
	//outline.m_outline = KazMath::Vec4<float>(0, 0, 0, 1);
	//m_ground.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	//m_ground.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	//m_ground.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	//m_ground.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;


	//outline.m_outline = KazMath::Vec4<float>(0, 0, 0, 1);
	//m_fence.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	//m_fence.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	//m_fence.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	//m_fence.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;


	//outline.m_outline = KazMath::Vec4<float>(0, 0, 0, 1);
	//m_tree.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	//m_tree.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	//m_tree.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	//m_tree.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;


	//outline.m_outline = KazMath::Vec4<float>(0, 0, 0, 1);
	//m_rock.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	//m_rock.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	//m_rock.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	//m_rock.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_ground.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_fence.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_tree.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_rock.Draw(arg_rasterize, arg_blasVec, m_stageTransform);


	//ImGui::Begin("Stage");

	////ImGui::DragFloat("POS_X", &BuildingMgr::Instance()->GetWall(2).lock()->m_initPos.x, 1.0f);
	////ImGui::DragFloat("POS_Y", &BuildingMgr::Instance()->GetWall(2).lock()->m_initPos.y, 1.0f);
	////ImGui::DragFloat("POS_Z", &BuildingMgr::Instance()->GetWall(2).lock()->m_initPos.z, 1.0f);
	////ImGui::DragFloat("SCALE_X", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.scale.x, 0.01f);
	////ImGui::DragFloat("SCALE_Y", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.scale.y, 0.01f);
	////ImGui::DragFloat("SCALE_Z", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.scale.z, 0.01f);
	////ImGui::DragFloat("ROTATE_X", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.rotation.x, 0.1f);
	////ImGui::DragFloat("ROTATE_Y", &BuildingMgr::Instance()->GetWall(2).lock()->m_rotateY, 0.1f);
	////ImGui::DragFloat("ROTATE_Z", &BuildingMgr::Instance()->GetWall(0).lock()->m_transform.rotation.z, 0.1f);

	////ImGui::Text(" ");

	////ImGui::DragFloat("EyeDistance", &m_cameraEyeDistance, 1.0f);
	//ImGui::SliderFloat("DirLight_X", &GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.x, 0.0f, 1.0f);
	//ImGui::SliderFloat("DirLight_Y", &GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.y, 0.0f, 1.0f);
	//ImGui::SliderFloat("DirLight_Z", &GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.z, 0.0f, 1.0f);

	//ImGui::Text("X : %f", m_player->GetPosZeroY().x);
	//ImGui::Text("Z : %f", m_player->GetPosZeroY().z);

	//ImGui::End();

}

int GameScene::SceneChange()
{
	return SCENE_NONE;
}
