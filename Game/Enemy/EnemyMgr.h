#pragma once
#include "../Game/DrawCallSet.h"
#include "../Enemy/EnemyRoute.h"
#include <array>
#include <memory>

class MineKuji;
class Core;
class Player;

class EnemyMgr {

private:

	static const int MAX_MINEKUJI = 16;
	std::array<std::shared_ptr<MineKuji>, MAX_MINEKUJI> m_minekujis;

public:

	EnemyMgr();

	void Init();

	void DebugGenerate();

	void Generate(EnemyRoute::ROUTE_ID arg_routeID);

	void GenerateMinekuji();

	void Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetTargetMineKujiIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);
	KazMath::Vec3<float> GetMineKujiScale(int arg_index);
	KazMath::Vec3<float> GetMineKujiTargetScale(int arg_index);
	KazMath::Vec3<float> GetMineKujiPos(int arg_index);
	std::weak_ptr<MineKuji> GetMineKuji(int arg_index);

};