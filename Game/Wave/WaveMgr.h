#pragma once
#include "../DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include "../DrawCallUISet.h"

class Wave;
class EnemyMgr;

class WaveMgr : public ISingleton<WaveMgr> {

private:

	std::vector<std::shared_ptr<Wave>> m_waves;		//�E�F�[�u�̔z��
	int m_waveCount;
	int m_nowWaveIndex;

	//�^�C�}�[��UI
	DrawCallUISet m_timerUI;
	DrawCallUISet m_frameUI;


public:

	void Setting();

	void Init(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//1Wave�ڂ�L��������B
	void GameStart();

	bool GetIsNight();
};