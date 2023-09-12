#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"
#include "../Game/DestructibleObject/DestructibleObjectMgr.h"
#include "../Game/WallAndTreeGeneratePos.h"
#include "../PointLightMgr.h"
#include "../Game/Rock/RockMgr.h"

Wave::Wave(int arg_dayTime, int arg_nightTime, std::vector<int> arg_tree, std::vector<int> arg_rock, std::vector<int> arg_mineralRock, std::vector<EnemyWaveInfo> arg_enemyWaveInfo)
{

	m_dayTime = arg_dayTime;
	m_nighTime = arg_nightTime;
	m_tree = arg_tree;
	m_rock = arg_rock;
	m_mineralRock = arg_mineralRock;
	m_enemyWaveInfo = arg_enemyWaveInfo;
	m_nowTime = 0;
	m_isNight = false;
	m_isActiveWave = false;


}

void Wave::Update(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//夜時間だったら
	if (m_isNight) {

		//この時間に湧く敵がいたら湧かせる。
		for (auto& enemy : m_enemyWaveInfo) {

			if (enemy.m_spawnFrame != m_nowTime) continue;

			//敵を湧かせる。
			if (enemy.m_enemyID == ENEMY_ID::MINEKUJI) {

				arg_enemyMgr.lock()->GenerateMinekuji(enemy.m_routeID);

			}
			else if (enemy.m_enemyID == ENEMY_ID::MINETSUMURI) {

				arg_enemyMgr.lock()->GenerateMinetsumuri(enemy.m_routeID, false);

			}
			else if (enemy.m_enemyID == ENEMY_ID::MINEKING) {

				arg_enemyMgr.lock()->GenerateMinetsumuri(enemy.m_routeID, true);

			}

		}

		//時間を進める。
		m_nowTime = std::clamp(m_nowTime + 1, 0, m_nighTime);

		//時間が終わったWaveを無効化する。
		if (m_nighTime <= m_nowTime) {

			Invalidate(arg_enemyMgr);

		}

	}
	//昼時間だったら
	else {


		//時間を進める。
		m_nowTime = std::clamp(m_nowTime + 1, 0, m_dayTime);

		//時間が終わったWaveを無効化する。
		if (m_dayTime <= m_nowTime) {

			//夜時間へ
			m_nowTime = 0;
			m_isNight = true;

		}

	}

	//ディレクショナルライトの方向を変えて昼夜を表現
	if (!m_isNight) {
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
	}
	else if (m_isNight) {
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
	}
	GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

	//ライトのアップデート
	PointLightMgr::Instance()->Update(m_isNight);

}

void Wave::Active()
{

	//ウェーブを有効化。
	m_isActiveWave = true;
	m_isNight = false;
	m_nowTime = 0;


	//木と岩をランダムで配置。
	for (auto& index : m_tree) {

		DestructibleObjectMgr::Instance()->GenerateTree(WallAndTreeGeneratePos::Instance()->m_treePos[index - 1]);

	}
	for (auto& index : m_rock) {

		RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[index - 1], {}, false, 2);

	}
	for (auto& index : m_mineralRock) {

		RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[index - 1], {}, true, 2);

	}

}

void Wave::Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//全ての敵を無効化。
	arg_enemyMgr.lock()->Init();

	//全ての破壊可能オブジェクトを削除。
	DestructibleObjectMgr::Instance()->Init();

	//Waveを終える。
	m_isActiveWave = false;

}

float Wave::WaveTimerRate()
{

	if (m_isNight) {

		return static_cast<float>(m_nowTime) / static_cast<float>(m_nighTime);

	}

	return static_cast<float>(m_nowTime) / static_cast<float>(m_dayTime);
}
