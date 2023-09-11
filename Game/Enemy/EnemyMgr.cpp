#include "EnemyMgr.h"
#include "../Game/Enemy/MineKuji.h"
#include "../Enemy/EnemyRoute.h"

EnemyMgr::EnemyMgr()
{

	for (auto& index : m_minekujis) {

		index = std::make_shared<MineKuji>();

	}

}

void EnemyMgr::Init()
{

	for (auto& index : m_minekujis) {

		index->Init();

	}

}

void EnemyMgr::Generate(EnemyRoute::ROUTE_ID arg_routeID)
{

	for (auto& index : m_minekujis) {

		if (index->GetIsActive()) continue;

		index->Generate(EnemyRoute::Instance()->GetRoute(arg_routeID));

		break;

	}

}

void EnemyMgr::DebugGenerate()
{

	//m_minekujis[0]->Generate(EnemyRoute::Instance()->GetRoute(EnemyRoute::C));

}

void EnemyMgr::Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player)
{

	for (auto& index : m_minekujis) {

		if (!index->GetIsActive()) continue;

		index->Update(arg_core, arg_player);

	}

}

void EnemyMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{


	for (auto& index : m_minekujis) {

		index->Draw(arg_rasterize, arg_blasVec);

	}

}
int EnemyMgr::GetTargetMineKujiIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance)
{

	//�������Ă����̂Ȃ�����^�[�Q�b�g�͈͓̔��̂��̂���������B
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_minekujis) {

		if (!index->GetIsActive()) continue;

		//�^�[�Q�b�g�̒��ɓ����Ă��邩���`�F�b�N����B
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPos()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//�C���f�b�N�X���擾�B
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_minekujis[0]));
		inTargetLength.emplace_back(length);

	}

	//�������ʂ�0��������͈͓��Ɋ₪�Ȃ��Ƃ������ƂȂ̂ŁA-1��Ԃ��B
	const int INDEX_COUNT = static_cast<int>(inTargetIndex.size());
	if (INDEX_COUNT <= 0) {

		return -1;

	}

	//�����������ʂ���ŒZ�̂��̂�I�ԁB
	int miniIndex = -1;
	float miniLength = arg_targetRange * 2.0f;
	for (int index = 0; index < INDEX_COUNT; ++index) {

		//�ۑ�����Ă���l���傫�������珈�����΂��B
		if (miniLength < inTargetLength[index]) continue;

		//�ۑ�����B
		miniLength = inTargetLength[index];
		miniIndex = inTargetIndex[index];

	}

	arg_targetDistance = miniLength;
	return miniIndex;

}

KazMath::Vec3<float> EnemyMgr::GetMineKujiScale(int arg_index)
{
	return m_minekujis[arg_index]->GetScale();
}

KazMath::Vec3<float> EnemyMgr::GetMineKujiTargetScale(int arg_index)
{
	return m_minekujis[arg_index]->GetTargetScale();
}

KazMath::Vec3<float> EnemyMgr::GetMineKujiPos(int arg_index)
{
	return m_minekujis[arg_index]->GetPosZeroY();
}

std::weak_ptr<MineKuji> EnemyMgr::GetMineKuji(int arg_index)
{
	return m_minekujis[arg_index];
}
