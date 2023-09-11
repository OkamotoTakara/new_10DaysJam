#include "Wall.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Game/Player.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/Effect/ShakeMgr.h"
#include "../Game/UI/NumberFont.h"

Wall::Wall()
{

	m_model[0].Load("Resource/Building/Wall/", "WallA.gltf");
	m_model[1].Load("Resource/Building/Wall/", "WallB.gltf");
	m_model[2].Load("Resource/Building/Wall/", "WallC.gltf");
	m_buildingBoxModel.Load("Resource/Building/", "BuildingBox.gltf");
	m_numberModel.Load("Resource/UI/NumFont/", "number.gltf");


	m_wallTransform.pos.y = -10000.0f;
	m_meshCollisionModel[0].Load("Resource/Building/Wall/", "WallACollision.gltf");
	m_meshCollisionModel[1].Load("Resource/Building/Wall/", "WallBCollision.gltf");
	m_meshCollisionModel[2].Load("Resource/Building/Wall/", "WallCCollision.gltf");
	m_meshCollider[0].Setting(m_meshCollisionModel[0].m_modelData->modelData[0].vertexData, m_wallTransform);
	m_meshCollider[1].Setting(m_meshCollisionModel[1].m_modelData->modelData[0].vertexData, m_wallTransform);
	m_meshCollider[2].Setting(m_meshCollisionModel[2].m_modelData->modelData[0].vertexData, m_wallTransform);

	Init();

}

void Wall::Init()
{

	m_isActive = false;
	m_isBuild = false;
	m_isReady = false;
	m_isOldReady = false;
	m_materialCounter = 0;
	m_sineWaveTimer = 0.0f;
	m_modelIndex = 0;
	m_easingTimer = EXIT_EASING_TIMER;
	m_noreadyStatus = NOREADY_STATUS::EXIT;
	m_wallTransform.pos.y = -1000.0f;
	m_meshCollider[m_modelIndex].Transform(m_wallTransform);
	m_hp = HP;

}

void Wall::Genrate(KazMath::Vec3<float> arg_generatePos, float arg_rotateY, int arg_modelIndex)
{

	m_transform.pos = arg_generatePos;
	m_initPos = arg_generatePos;
	Init();

	m_isActive = true;
	m_rotateY = arg_rotateY;
	m_boxTransform.rotation.y = arg_rotateY;
	m_modelIndex = arg_modelIndex;

}

void Wall::Update(std::weak_ptr<Player> arg_player)
{

	//建築したら
	if (m_isBuild) {

		switch (m_buildStatus)
		{
		case Wall::BUILD_STATUS::UPPER:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, UPPER_EASING_TIMER);
			float easingAmount = EasingMaker(Out, Cubic, m_easingTimer / UPPER_EASING_TIMER);
			m_transform.pos.y = m_initPos.y + UPPER_DISTANCE * easingAmount;
			m_transform.scale = { 2.0f + easingAmount * 2.0f,2.0f + easingAmount * 2.0f ,2.0f + easingAmount * 2.0f };
			if (UPPER_EASING_TIMER <= m_easingTimer) {

				m_easingTimer = 0.0f;
				m_buildStatus = BUILD_STATUS::STAY;

			}
		}
		break;
		case Wall::BUILD_STATUS::STAY:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, BUILD_STAY_EASING_TIMER);
			if (BUILD_STAY_EASING_TIMER <= m_easingTimer) {

				m_easingTimer = 0.0f;
				m_buildStatus = BUILD_STATUS::DOWN;

			}
		}
		break;
		case Wall::BUILD_STATUS::DOWN:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, DOWN_EASING_TIMER);
			float easingAmount = EasingMaker(In, Exp, m_easingTimer / DOWN_EASING_TIMER);
			m_transform.pos.y = m_initPos.y + UPPER_DISTANCE - (UPPER_DISTANCE * easingAmount);
			if (DOWN_EASING_TIMER <= m_easingTimer) {

				m_easingTimer = 0.0f;
				m_buildStatus = BUILD_STATUS::COMPLETE;

				ShakeMgr::Instance()->m_shakeAmount = 5.0f;

				m_wallTransform = m_transform;
				m_meshCollider[m_modelIndex].Setting(m_meshCollisionModel[m_modelIndex].m_modelData->modelData[0].vertexData, m_wallTransform);

			}
		}
		break;
		case Wall::BUILD_STATUS::COMPLETE:
		{
		}
		break;
		default:
			break;
		}

		//Y軸回転を元に戻す。
		m_transform.rotation.y += (m_rotateY - m_transform.rotation.y) / 5.0f;

	}
	//まだ建築していなかったら
	else {

		//場所を設定。
		m_transform.pos = m_initPos;
		m_boxTransform.pos = m_initPos;

		//近づいたらReady状態にする。
		float distance = KazMath::Vec3<float>(GetPosZeroY() - arg_player.lock()->GetPosZeroY()).Length();
		m_isOldReady = m_isReady;
		if (distance < arg_player.lock()->GetMineralAffectRange()) {
			m_isReady = true;
		}
		else {
			m_isReady = false;
		}

		//Readyになった瞬間だったら
		if (!m_isOldReady && m_isReady) {
			m_noreadyStatus = NOREADY_STATUS::APPEAR;
			m_easingTimer = 0.0f;
			m_transform.rotation.y = m_rotateY + 360.0f * 2.0f;
		}
		//Readyが解除された瞬間だったrあ
		if (m_isOldReady && !m_isReady) {
			m_noreadyStatus = NOREADY_STATUS::EXIT;
			m_easingTimer = 0.0f;
		}

		//ステータスによって大きさを変えたりする。
		switch (m_noreadyStatus)
		{
		case Wall::NOREADY_STATUS::APPEAR:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, APPEAR_EASING_TIMER);
			float easingAmount = EasingMaker(Out, Back, m_easingTimer / APPEAR_EASING_TIMER);
			float scale = 2.0f * easingAmount;
			m_transform.scale = { scale ,scale ,scale };
			m_boxTransform.scale = { 2.0f - scale ,2.0f - scale ,2.0f - scale };

			if (APPEAR_EASING_TIMER <= m_easingTimer) {

				m_easingTimer = 0.0f;
				m_noreadyStatus = NOREADY_STATUS::STAY;

			}

		}
		break;
		case Wall::NOREADY_STATUS::STAY:
		{


		}
		break;
		case Wall::NOREADY_STATUS::EXIT:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, EXIT_EASING_TIMER);
			float easingAmount = EasingMaker(In, Back, m_easingTimer / EXIT_EASING_TIMER);
			float scale = 2.0f - 2.0f * easingAmount;
			m_transform.scale = { scale ,scale ,scale };
			m_boxTransform.scale = { 2.0f - scale ,2.0f - scale ,2.0f - scale };

		}
		break;
		default:
			break;
		}

		//サイン波の動きを更新。
		m_sineWaveTimer += ADD_SINE_WAVE_TIMER;

		if (MATERIAL_COUNT <= m_materialCounter || KeyBoradInputManager::Instance()->InputTrigger(DIK_B)) {
			m_isBuild = true;
			m_buildStatus = BUILD_STATUS::UPPER;
			m_easingTimer = 0.0f;
		}

		//Y軸回転を元に戻す。
		m_transform.rotation.y += (m_rotateY - m_transform.rotation.y) / 20.0f;

	}

	//HPが0になったら初期化
	if (m_hp <= 0) {
		Init();
	}

}

void Wall::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	KazMath::Transform3D modelTransform = m_transform;
	modelTransform.pos.y += std::sin(m_sineWaveTimer) * SINE_WAVE_MOVE;

	KazMath::Transform3D boxTransform = m_boxTransform;
	boxTransform.scale += KazMath::Vec3<float>(std::sin(m_sineWaveTimer) * BOX_SINE_WAVE_SCALE, std::sin(m_sineWaveTimer) * BOX_SINE_WAVE_SCALE, std::sin(m_sineWaveTimer) * BOX_SINE_WAVE_SCALE);

	if (!m_isActive) {

		boxTransform.pos = { 0,-10000,0 };
		boxTransform.scale = { 0,0,0 };

		modelTransform.pos = { 0,-10000,0 };
		modelTransform.scale = { 0,0,0 };

	}

	if (m_noreadyStatus == NOREADY_STATUS::STAY) {
		boxTransform = m_boxTransform;
	}

	if (m_isBuild) {
		boxTransform.scale = { 0,0,0 };
	}

	if (m_isReady && !m_isBuild) {

		m_numberModelTransform.pos = boxTransform.pos;
		m_numberModelTransform.pos.y += 30.0f;
		m_numberModelTransform.scale.x = 5.0f;
		m_numberModelTransform.scale.z = 5.0f;
		m_numberModelTransform.scale.y += (5.0f - m_numberModelTransform.scale.y) / 5.0f;

	}
	else {

		m_numberModelTransform.pos = boxTransform.pos;
		m_numberModelTransform.pos.y += 30.0f;
		m_numberModelTransform.scale.x = 5.0f;
		m_numberModelTransform.scale.z = 5.0f;
		m_numberModelTransform.scale.y += (0.0f - m_numberModelTransform.scale.y) / 5.0f;
		if (m_numberModelTransform.scale.y < 0.5f) {
			m_numberModelTransform.scale.x = 0.0f;
			m_numberModelTransform.scale.y = 0.0f;
			m_numberModelTransform.scale.z = 0.0f;
		}
	}

	m_numberModel.m_model.materialBuffer.front().front() = NumberFont::Instance()->m_font[MATERIAL_COUNT - m_materialCounter];

	m_model[m_modelIndex].Draw(arg_rasterize, arg_blasVec, modelTransform);
	m_buildingBoxModel.Draw(arg_rasterize, arg_blasVec, boxTransform);
	m_numberModel.Draw(arg_rasterize, arg_blasVec, m_numberModelTransform, 0, false);

}

MeshCollision::CheckHitResult Wall::CheckHitMesh(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir) {

	return m_meshCollider[m_modelIndex].CheckHitRay(arg_pos, arg_dir);

}

KazMath::Vec3<float> Wall::GetPosZeroY()
{
	return KazMath::Vec3<float>(m_initPos.x, 0.0f, m_initPos.z);
}
