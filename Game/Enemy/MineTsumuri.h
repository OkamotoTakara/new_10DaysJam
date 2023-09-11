#pragma once
#include "../Game/DrawCallSet.h"

class Core;
class Player;
class Mineral;

class MineTsumuri {

private:

	//基本情報
	DrawCallSet m_model;
	DrawCallSet m_shellModel;
	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Transform3D m_shellTransform;
	KazMath::Vec3<float> m_forwardVec;
	float m_scale;
	bool m_isActive;
	float m_gravity;
	const float GRAVITY = 0.1f;
	int m_hp;
	const int HP = 9;
	const float SCALE = 3.0f;

	//状態ごとの変数
	enum Mode {
		CoreAttack,
		MineralAttack,
		WallAttack,
		PlayerAttack,
	}m_mode;

	//コアを攻撃する変数
	const float CORE_ATTACK_RANGE = 40.0f;
	KazMath::Vec3<float> m_coreAttackReactionVec;
	float m_coreAttackMoveSpeed;
	int m_coreAttackDelayTimer;
	int m_coreAttackDelay;
	const int MIN_CORE_ATTACK_DELAY = 60;
	const int MAX_CORE_ATTACK_DELAY = 90;
	const float ADD_CORE_ATTACK_SPEED = 0.1f;
	const float MAX_CORE_ATTACK_SPEED = 2.0f;
	enum ATTACK_ID {
		ATTACK,
		STAY,
	}m_attackID;

	//探知範囲
	const float ENEMY_SEARCH_RANGE = 40.0f;
	const float ENEMY_SEARCH_END_RANGE = 60.0f;

	//コアに向かっていく変数
	const float CORE_MOVE_SPEED = 1.0f;	//コアに向かって行く初速度
	const float CORE_MOVE_DELAY = 60.0f;
	float m_coreMoveDelayTimer;
	float m_coreMoveSpeed;
	bool m_isAttackCore;
	bool m_isAttackMineral;
	bool m_isAttackPlayer;

	//攻撃された場合の変数
	bool m_isAttackedMineral;
	KazMath::Vec3<float> m_attackedReactionVec;
	std::weak_ptr<Mineral> m_attackedMineral;

	//攻撃を受けたときの拡縮
	float m_attackedScale;
	const float ATTACKED_SCALE = 4.5f;

	//進路AI
	std::vector<KazMath::Vec3<float>> m_route;
	int m_passedRouteIndex;
	const float ROUTE_CHECK_SCALE = 30.0f;

	//壁攻撃
	int m_wallIndex;
	bool m_isAttackWall;

	//殻に関数変数
	bool m_isShell;		//殻がついているか。
	bool m_inShell;		//殻にこもっているか。
	float m_inShellTimer;
	const float IN_SHELL_TIMER = 120.0f;
	float m_shellPosY;
	const float SHELL_POS_Y = 8.0f;
	float m_shellGravity;
	float m_shellHP;
	const float SHELL_HP = 10.0f;
	KazMath::Vec3<float> m_shellBreakVel;
	const float SHELL_BRWAK_VEL = 5.0f;
	KazMath::Vec3<float> m_shellBreakRightVec;
	float m_shellBreakRotation;


public:

	MineTsumuri();

	void Init();

	void Generate(std::vector<KazMath::Vec3<float>> arg_route);

	void Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetHP() { return m_hp; }
	void Damage(std::weak_ptr<Mineral> arg_mineral, int arg_damage = 1);

	bool GetIsActive() { return m_isActive; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }
	KazMath::Vec3<float> GetTargetScale() { return KazMath::Vec3<float>(10.0f, 10.0f, 10.0f); }

private:

	void AttackCore(std::weak_ptr<Core> arg_core);
	void AttackMineral();
	void AttackPlayer(std::weak_ptr<Player> arg_player);
	void AttackWall();
	void CheckHitPlayer(std::weak_ptr<Player> arg_player);
	KazMath::Vec3<float> SearchRoute();

	void Move();
	void Rotation(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player);
	void CheckHit(std::weak_ptr<Player> arg_player);
	void UpdateShell();

};