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
		EnemyRoute::ROUTE_ID m_routeID;	//敵が湧く場所のID
		ENEMY_ID m_enemyID;
		int m_spawnFrame; //敵が湧くフレーム
		EnemyWaveInfo(EnemyRoute::ROUTE_ID arg_routeID, ENEMY_ID arg_enemyID, int arg_spawnFrame) : m_routeID(arg_routeID), m_enemyID(arg_enemyID), m_spawnFrame(arg_spawnFrame) {};
	};

private:

	int m_dayTime;		//日中のフレーム数 0からスタート
	int m_nighTime;		//夜のフレーム数 0からスタート
	int m_nowTime;		//現在の経過フレーム
	int m_treeCount;
	int m_rockCount;
	bool m_isNight;
	bool m_isActiveWave;
	std::vector<EnemyWaveInfo> m_enemyWaveInfo;	//敵を湧かせるフレーム数

public:

	Wave(int a, int arg_nightTime, int arg_treeCount, int arg_rockCount, std::vector<EnemyWaveInfo> arg_enemyWaveInfo);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	//ウェーブを有効化
	void Active();
	//ウェーブを無効化
	void Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	float WaveTimerRate();

	bool GetIsActiveWave() { return m_isActiveWave; }
	bool GetIsNight() { return m_isNight; }

};