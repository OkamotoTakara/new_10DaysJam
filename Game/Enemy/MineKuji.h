#pragma once
#include "../Game/DrawCallSet.h"

class Core;
class Player;
class Mineral;

class MineKuji {

private:

	//Šî–{î•ñ
	DrawCallSet m_model;
	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_forwardVec;
	float m_scale;
	bool m_isActive;
	float m_gravity;
	const float GRAVITY = 0.1f;
	int m_hp;
	const int HP = 9;
	const float SCALE = 5.0f;

	//ó‘Ô‚²‚Æ‚Ì•Ï”
	enum Mode {
		CoreAttack,
		MineralAttack,
		WallAttack,
	}m_mode;

	//ƒRƒA‚ğUŒ‚‚·‚é•Ï”
	const float CORE_ATTACK_RANGE = 40.0f;
	KazMath::Vec3<float> m_coreAttackReactionVec;
	float m_coreAttackMoveSpeed;
	int m_coreAttackDelayTimer;
	int m_coreAttackDelay;
	const int MIN_CORE_ATTACK_DELAY = 60;
	const int MAX_CORE_ATTACK_DELAY = 90;
	const float ADD_CORE_ATTACK_SPEED = 0.05f;
	const float MAX_CORE_ATTACK_SPEED = 1.0f;
	enum ATTACK_ID {
		ATTACK,
		STAY,
	}m_attackID;

	//ƒRƒA‚ÉŒü‚©‚Á‚Ä‚¢‚­•Ï”
	const float CORE_MOVE_SPEED = 1.0f;	//ƒRƒA‚ÉŒü‚©‚Á‚Äs‚­‰‘¬“x
	const float CORE_MOVE_DELAY = 90.0f;
	float m_coreMoveDelayTimer;
	float m_coreMoveSpeed;
	bool m_isAttackCore;
	bool m_isAttackMineral;

	//UŒ‚‚³‚ê‚½ê‡‚Ì•Ï”
	bool m_isAttackedMineral;
	KazMath::Vec3<float> m_attackedReactionVec;
	std::weak_ptr<Mineral> m_attackedMineral;

	//UŒ‚‚ğó‚¯‚½‚Æ‚«‚ÌŠgk
	float m_attackedScale;
	const float ATTACKED_SCALE = 4.5f;

	//i˜HAI
	std::vector<KazMath::Vec3<float>> m_route;
	int m_passedRouteIndex;
	const float ROUTE_CHECK_SCALE = 30.0f;

	//•ÇUŒ‚
	int m_wallIndex;
	bool m_isAttackWall;


public:

	MineKuji();

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
	void AttackWall();
	void CheckHitPlayer(std::weak_ptr<Player> arg_player);
	KazMath::Vec3<float> SearchRoute();

};