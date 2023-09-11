#pragma once
#include "../Game/DrawCallSet.h"

class Player;

class Rock {

public:

	enum ROCK_ID {
		SMALL,
		MEDIUM,
		BIG,
	};

private:

	//基本情報
	std::array<DrawCallSet, 3> m_model;
	int m_modelIndex;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_respawnVec;
	float m_gravity;
	bool m_canCollision;
	bool m_canOldCollision;
	const float GRAVITY = 0.1f;
	const float STRONG_DAIPAN_POWER = 1.0f;
	int m_hp;
	std::array<int, 3> MAX_HP = { 3, 6, 9 };
	KazMath::Vec3<float> m_reactionVec;

	//ステータス
	bool m_isAlive;
	ROCK_ID m_rockID;

	//サイズ関係 描画
	const float DRAW_SIZE_BIG = 6.5f;
	const float DRAW_SIZE_MEDIUM = 5.5f;
	const float DRAW_SIZE_SMALL = 3.5f;
	//当たり判定
	const float COLLISION_SIZE_BIG = 6.0f;
	const float COLLISION_SIZE_MEDIUM = 4.6f;
	const float COLLISION_SIZE_SMALL = 3.5f;

public:


	Rock();

	void Init();

	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnVec, int arg_rockID = BIG);

	void Update(std::weak_ptr<Player> arg_player, std::vector<std::pair<KazMath::Vec3<float>, int>>& arg_respawnVec);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetHP() { return m_hp; }
	void Damage(KazMath::Vec3<float> arg_reactionVec, int arg_damage = 1);

	bool GetIsAlive() { return m_isAlive; }
	ROCK_ID GetRockID() { return m_rockID; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }

};