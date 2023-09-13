#include "WaveMgr.h"
#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"
#include "../Game/TitleFlag.h"
#include "../Game/ResultFlag.h"
#include "../Game/Tutorial.h"

void WaveMgr::Setting()
{

	//現在のウェーブ数を初期化。
	m_nowWaveIndex = 0;

	//生成する敵の情報配列
	std::vector<Wave::EnemyWaveInfo> enemyInfo;
	//ウェーブに必要な情報
	int dayTime;		//日中の時間 フレーム数
	int nightTime;	//夜の時間 フレーム数
	std::vector<int> tree = {  };		//有効化時に生成される木のIndex 1スタート
	std::vector<int> rock = {  };		//有効化時に生成される岩のIndex 1スタート
	std::vector<int> mineralRock = {  };		//有効化時に生成されるミネラル岩のIndex 1スタート


	//チュートリアルのウェーブ-----------------------------------------------------------------------------------------
	dayTime = 60;
	nightTime = 1800;
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	//enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);
	//ウェーブを追加。
	m_tutorialWave = std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo);
	enemyInfo.clear();

	
	//1ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 600;		//日中の時間 フレーム数
	nightTime = 1800;	//夜の時間 フレーム数
	tree = {  };		//有効化時に生成される木のIndex 1スタート
	rock = { 3 };		//有効化時に生成される岩のIndex 1スタート
	mineralRock = { 1,2 };		//有効化時に生成されるミネラル岩のIndex 1スタート
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	//enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 180);
	//enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	//enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);
	//ウェーブを追加。
	m_wave1 = std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo);
	enemyInfo.clear();

	m_waves.emplace_back(m_wave1);

	
	//2ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 3200;		//日中の時間 フレーム数
	nightTime = 3200;	//夜の時間 フレーム数
	tree = { 4 };		//有効化時に生成される木の数
	rock = {  };		//有効化時に生成される岩の数
	mineralRock = { 3 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1440);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1620);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1800);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//3ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 3200;		//日中の時間 フレーム数
	nightTime = 3200;	//夜の時間 フレーム数
	tree = { 2 };		//有効化時に生成される木の数
	rock = {  };		//有効化時に生成される岩の数
	mineralRock = { 1 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 900);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();



	//4ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 3200;		//日中の時間 フレーム数
	nightTime = 3200;	//夜の時間 フレーム数
	tree = { 3,8 };		//有効化時に生成される木の数
	rock = { };		//有効化時に生成される岩の数
	mineralRock = { 2,6 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//5ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 3200;		//日中の時間 フレーム数
	nightTime = 3200;	//夜の時間 フレーム数
	tree = { 6 };		//有効化時に生成される木の数
	rock = { 4 };		//有効化時に生成される岩の数
	mineralRock = {  };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 360);


	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 360);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//6ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 3200;		//日中の時間 フレーム数
	nightTime = 3200;	//夜の時間 フレーム数
	tree = { 6 };		//有効化時に生成される木の数
	rock = { 4 };		//有効化時に生成される岩の数
	mineralRock = {  };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);


	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKING, 0);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//7ウェーブ目 -----------------------------------------------------------------------------------------
	dayTime = 3200;		//日中の時間 フレーム数
	nightTime = 6400;	//夜の時間 フレーム数
	tree = { 4,6 };		//有効化時に生成される木の数
	rock = { 4,7 };		//有効化時に生成される岩の数
	mineralRock = {  };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 1080);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKING, 0);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 1080);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKING, 900);

	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//ウェーブ数を保存。
	m_waveCount = static_cast<int>(m_waves.size());

	m_isFinishAllWave = false;

	//UIをロード
	m_timerUI.Load("Resource/UI/Timer/Timer.png");
	m_frameUI.Load("Resource/UI/Timer/Frame.png");

}

void WaveMgr::Init(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	m_nowWaveIndex = 0;
	for (auto& wave : m_waves) {

		wave->Invalidate(arg_enemyMgr);

	}

}

void WaveMgr::Update(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//ウェーブを更新。
	m_waves[m_nowWaveIndex]->Update(arg_enemyMgr);

	//現在のウェーブが終わっていたら次のウェーブへ
	if (!m_waves[m_nowWaveIndex]->GetIsActiveWave()) {

		//次のウェーブが存在しているか？
		if (m_waveCount - 1 <= m_nowWaveIndex) {

			//次のウェーブがない時
			m_isFinishAllWave = true;

		}
		else {

			++m_nowWaveIndex;
			m_waves[m_nowWaveIndex]->Active();

		}

	}

	//UIを更新。
	m_timerUI.m_transform.scale = { 168,168 };
	m_timerUI.m_transform.pos = { 128,128 };
	if (m_waves[m_nowWaveIndex]->GetIsNight()) {
		m_timerUI.m_transform.rotation = 180.0f + m_waves[m_nowWaveIndex]->WaveTimerRate() * 180.0f - 90.0f;
	}
	else {
		m_timerUI.m_transform.rotation = m_waves[m_nowWaveIndex]->WaveTimerRate() * 180.0f - 90.0f;
	}
	m_frameUI.m_transform.scale = { 168,168 };
	m_frameUI.m_transform.pos = { 128,128 };

}

void WaveMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{


	if (0 < ResultFlag::Instance()->m_uiDeleteTime) {


		m_frameUI.m_color.color.a += static_cast<int>((0.0f - m_frameUI.m_color.color.a) / 3.0f);
		m_timerUI.m_color.color.a += static_cast<int>((0.0f - m_timerUI.m_color.color.a) / 3.0f);

		if (m_frameUI.m_color.color.a <= 50.0f) {

			m_frameUI.m_color.color.a = 0;
			m_timerUI.m_color.color.a = 0;

		}
		else {

			//UIを描画。
			m_frameUI.Draw(arg_rasterize);
			m_timerUI.Draw(arg_rasterize);

		}



	}
	else if ((!TitleFlag::Instance()->m_isTitle && !ResultFlag::Instance()->m_isResult)) {

		//UIを描画。
		m_frameUI.Draw(arg_rasterize);
		m_timerUI.Draw(arg_rasterize);

		m_frameUI.m_color.color.a += static_cast<int>((255.0f - m_frameUI.m_color.color.a) / 15.0f);
		m_timerUI.m_color.color.a += static_cast<int>((255.0f - m_timerUI.m_color.color.a) / 15.0f);

	}
	else {

		m_frameUI.m_color.color.a = 0;
		m_timerUI.m_color.color.a = 0;

	}

}

void WaveMgr::GameStart()
{

	//チュートリアルだったら最初のウェーブを差し替え。
	if (Tutorial::Instance()->is_tutorial) {
		m_waves.front() = m_tutorialWave;
	}
	else {
		m_waves.front() = m_wave1;
	}

	m_waves.front()->Active();

	m_isFinishAllWave = false;

}

bool WaveMgr::GetIsNight()
{
	return m_waves[m_nowWaveIndex]->GetIsNight();
}

void WaveMgr::SetTime(int wave_index, int wave_time)
{
	m_waves[wave_index]->SetMNowTime(wave_time);
}
