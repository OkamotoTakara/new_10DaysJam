#pragma once
#include "../Game/DrawCallSet.h"

class Player;
class Rock;
class MineKuji;
class DestructibleTree;
class BuildingMaterial;

class Mineral {

public:

	enum MINERAL_ID {
		SMALL,
		MEDIUM,
		BIG,
	};

private:

	//��{���
	std::array<DrawCallSet, 3> m_model;
	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_forwardVec;			//���ʃx�N�g��
	DrawCallSet m_surprisedModel;
	KazMath::Transform3D m_surpisedTransform;
	KazMath::Vec3<float> m_respawnVec;
	float m_gravity;
	const float GRAVITY = 0.1f;
	float m_gatheringTriggerEffect;
	const float GATHERING_TRIGGER_EFFECT = 2.0f;
	const float STRONG_DAIPAN_POWER = 2.0f;
	const float GATHERING_DAIPAN_POWER = 1.0f;	//���łɒǏ]���Ă���Ƃ��́A�����p�̗�
	int m_hp;
	std::array<int, 3> HP = { 3, 6, 9 };
	std::weak_ptr<Mineral> m_thisMineral;	//�������g

	//���U����Ƃ��̃x�N�g��
	KazMath::Vec3<float> m_breakUpVec;
	const float BREAK_UP_POWER = 1.5f;

	//�X�e�[�^�X
	bool m_isAlive;
	bool m_isOldGathering;
	bool m_isGathering;		//�����g��ŋ����Ԃ��B
	bool m_canGathering;	//����s�\�t���O�B���X�|�[�����Ă���ڒn����܂ł͑���ɓ���Ȃ��B
	MINERAL_ID m_mineralID;

	//���ނ��^��ł��鎞
	std::weak_ptr<BuildingMaterial> m_haveMaterial;
	bool m_isGoToGetMaterial;

	//�T�C�Y�֌W �`��
	const float MINERAL_DRAW_SIZE_BIG = 7.0f;
	const float MINERAL_DRAW_SIZE_MEDIUM = 4.5f;
	const float MINERAL_DRAW_SIZE_SMALL = 3.0f;
	//�����蔻��
	const float MINERAL_COLLISION_SIZE_BIG = 7.0f;
	const float MINERAL_COLLISION_SIZE_MEDIUM = 5.0f;
	const float MINERAL_COLLISION_SIZE_SMALL = 3.0f;
	//�T�C�Y���Ƃɖ��܂�̑΍�̂��߂�Y���������グ��ʁB
	std::array<const float, 3> MINERAL_OFFSETY = { 1.0f, 5.0f, 10.0f };

	//�U���֌W
	std::weak_ptr<Rock> m_attackTargetRock;
	std::weak_ptr<MineKuji> m_attackTargetMineKuji;
	std::weak_ptr<DestructibleTree> m_attackDestrutibleTree;
	KazMath::Vec3<float> m_attackReactionVec;
	float m_attackMoveSpeed;
	int m_stayDelayTimer;
	int m_stayDelay;
	const int MIN_STAY_DELAY = 60;
	const int MAX_STAY_DELAY = 90;
	const float ADD_ATTACK_SPEED = 0.05f;
	const float MAX_ATTACK_SPEED = 1.0f;
	bool m_isAttack;
	enum ATTACK_ID {
		ATTACK,
		STAY,
	}m_attackID;

	//�r�b�N���}�[�N�֘A
	bool m_isDrawSurpised;	//�r�b�N���}�[�N��`�悷�邩�̔��f�B
	float m_surpisedTimer;
	enum class SURPRISED_STATUS {
		APPEAR,
		STAY,
		EXIT,
	}m_surprisedStatus;
	const float SURPRISED_APPEAR_TIMER = 15.0f;
	const float SURPRISED_STAY_TIMER = 10.0f;
	const float SURPRISED_EXIT_TIMER = 15.0f;


public:

	Mineral();

	void Init();

	void Generate(std::weak_ptr<Mineral> arg_thisMineral, KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnVec, int arg_mineralID = BIG);

	void Update(std::weak_ptr<Player> arg_player, std::vector<std::pair<KazMath::Vec3<float>, int>>& arg_respawnVec);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void Damage(int arg_damage = 1);
	int GetHP() { return m_hp; }

	//�d�Ȃ�΍�B
	void CounterOverlap(KazMath::Vec3<float> arg_centerPos, KazMath::Vec3<float>& arg_mineralPos, float arg_scale);

	//���U������B
	void BreakUP(KazMath::Vec3<float> arg_forwardVec);

	//�U������B
	void Attack(std::weak_ptr<Rock> arg_attackTargetRock);
	void Attack(std::weak_ptr<MineKuji> arg_attackTargetMinekuji);
	void Attack(std::weak_ptr<DestructibleTree> arg_destructibleTree);

	//�}�e���A�������B
	void HaveMaterial(std::weak_ptr<BuildingMaterial> arg_material);

	//�}�e���A���𗎂Ƃ��B
	void DropMaterial();
	void EraseTheMaterial();

	bool GetIsAlive() { return m_isAlive; }
	bool GetIsGathering() { return m_isGathering; }
	bool GetIsGatheringTrigger() { return !m_isOldGathering && m_isGathering; }
	bool GetIsAttack() { return m_isAttack; }
	bool GetHaveMaterial() { return !m_haveMaterial.expired(); }
	MINERAL_ID GetMineralID() { return m_mineralID; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }
	float GetCollisionScale();
	KazMath::Vec3<float>& GetPosRef() { return m_transform.pos; }

private:

	void UpdateAttack(std::weak_ptr<Player> arg_player);
	void UpdateHaveMaterial(std::weak_ptr<Player> arg_player);

};