#pragma once
#include "../DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include "../DrawCallUISet.h"

class Wave;
class EnemyMgr;

class WaveMgr : public ISingleton<WaveMgr> {

private:

	std::vector<std::shared_ptr<Wave>> m_waves;		//ウェーブの配列
	int m_waveCount;
	int m_nowWaveIndex;
	bool m_isFinishAllWave;

	//ウェーブ
	std::shared_ptr<Wave> m_wave1;	//通常の1Wave目
	std::shared_ptr<Wave> m_tutorialWave;	//チュートリアル用のウェーブ1

	//タイマーのUI
	DrawCallUISet m_timerUI;
	DrawCallUISet m_frameUI;


public:

	void Setting();

	void Init(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//1Wave目を有効化する。
	void GameStart();

	bool GetIsNight();

	void SetTime(int wave_index, int wave_time);

	bool GetIsFinishAllWave() { return m_isFinishAllWave; }

	int GetNowWaveIndex() { return m_nowWaveIndex; }
};