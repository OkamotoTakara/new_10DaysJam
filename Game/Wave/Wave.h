#pragma once
#include "../Game/DrawCallSet.h"
#include "../Enemy/EnemyRoute.h"

class EnemyMgr;

class Wave {

public:

	enum class ENEMY_ID {
		MINEKUJI,
		MINETSUMURI,
	};

	struct EnemyWaveInfo {
		EnemyRoute::ROUTE_ID m_routeID;	//�G���N���ꏊ��ID
		ENEMY_ID m_enemyID;
		int m_spawnFrame; //�G���N���t���[��
		EnemyWaveInfo(EnemyRoute::ROUTE_ID arg_routeID, ENEMY_ID arg_enemyID, int arg_spawnFrame) : m_routeID(arg_routeID), m_enemyID(arg_enemyID), m_spawnFrame(arg_spawnFrame) {};
	};

private:

	int m_dayTime;		//�����̃t���[���� 0����X�^�[�g
	int m_nighTime;		//��̃t���[���� 0����X�^�[�g
	int m_nowTime;		//���݂̌o�߃t���[��
	int m_treeCount;
	int m_rockCount;
	bool m_isNight;
	bool m_isActiveWave;
	std::vector<EnemyWaveInfo> m_enemyWaveInfo;	//�G��N������t���[����

public:

	Wave(int a, int arg_nightTime, int arg_treeCount, int arg_rockCount, std::vector<EnemyWaveInfo> arg_enemyWaveInfo);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	//�E�F�[�u��L����
	void Active();
	//�E�F�[�u�𖳌���
	void Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	float WaveTimerRate();

	bool GetIsActiveWave() { return m_isActiveWave; }
	bool GetIsNight() { return m_isNight; }

};