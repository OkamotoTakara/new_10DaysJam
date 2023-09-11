#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"
#include "../Game/DestructibleObject/DestructibleObjectMgr.h"
#include "../Game/WallAndTreeGeneratePos.h"
#include "../PointLightMgr.h"
#include "../Game/Rock/RockMgr.h"

Wave::Wave(int arg_dayTime, int arg_nightTime, int arg_treeCount, int arg_rockCount, std::vector<EnemyWaveInfo> arg_enemyWaveInfo)
{

	m_dayTime = arg_dayTime;
	m_nighTime = arg_nightTime;
	m_treeCount = arg_treeCount;
	m_rockCount = arg_rockCount;
	m_enemyWaveInfo = arg_enemyWaveInfo;
	m_nowTime = 0;
	m_isNight = false;
	m_isActiveWave = false;


}

void Wave::Update(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//�鎞�Ԃ�������
	if (m_isNight) {

		//���̎��ԂɗN���G��������N������B
		for (auto& enemy : m_enemyWaveInfo) {

			if (enemy.m_spawnFrame != m_nowTime) continue;

			//�G��N������B
			if (enemy.m_enemyID == ENEMY_ID::MINEKUJI) {

				arg_enemyMgr.lock()->GenerateMinekuji(enemy.m_routeID);

			}
			else if (enemy.m_enemyID == ENEMY_ID::MINETSUMURI) {

				arg_enemyMgr.lock()->GenerateMinetsumuri(enemy.m_routeID);

			}

		}

		//���Ԃ�i�߂�B
		m_nowTime = std::clamp(m_nowTime + 1, 0, m_nighTime);

		//���Ԃ��I�����Wave�𖳌�������B
		if (m_nighTime <= m_nowTime) {

			Invalidate(arg_enemyMgr);

		}

	}
	//�����Ԃ�������
	else {


		//���Ԃ�i�߂�B
		m_nowTime = std::clamp(m_nowTime + 1, 0, m_dayTime);

		//���Ԃ��I�����Wave�𖳌�������B
		if (m_dayTime <= m_nowTime) {

			//�鎞�Ԃ�
			m_nowTime = 0;
			m_isNight = true;

		}

	}

	//�f�B���N�V���i�����C�g�̕�����ς��Ē����\��
	if (!m_isNight) {
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
	}
	else if (m_isNight) {
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
	}
	GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

	//���C�g�̃A�b�v�f�[�g
	PointLightMgr::Instance()->Update(m_isNight);

}

void Wave::Active()
{

	//�E�F�[�u��L�����B
	m_isActiveWave = true;
	m_isNight = false;
	m_nowTime = 0;

	//�؂Ɗ�������_���Ŕz�u�B
	for (int index = 0; index < m_treeCount; ++index) {

		//�����ʒu�����肵�Đ����B
		int generatePos = KazMath::Rand(0, WallAndTreeGeneratePos::Instance()->m_treeCount - 1);

		DestructibleObjectMgr::Instance()->GenerateTree(WallAndTreeGeneratePos::Instance()->m_treePos[generatePos]);

	}
	for (int index = 0; index < m_rockCount; ++index) {

		//�����ʒu�����肵�Đ����B
		int generatePos = KazMath::Rand(0, WallAndTreeGeneratePos::Instance()->m_rockCount - 1);

		RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[generatePos], {}, 2);

	}

}

void Wave::Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//�S�Ă̓G�𖳌����B
	arg_enemyMgr.lock()->Init();

	//�S�Ă̔j��\�I�u�W�F�N�g���폜�B
	DestructibleObjectMgr::Instance()->Init();

	//Wave���I����B
	m_isActiveWave = false;

}

float Wave::WaveTimerRate()
{

	if (m_isNight) {

		return static_cast<float>(m_nowTime) / static_cast<float>(m_nighTime);

	}

	return static_cast<float>(m_nowTime) / static_cast<float>(m_dayTime);
}
