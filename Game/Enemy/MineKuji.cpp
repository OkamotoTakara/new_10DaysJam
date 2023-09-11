#include "MineKuji.h"
#include "../Game/Core/Core.h"
#include "../Effect/ShakeMgr.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Game/Player.h"
#include "../Game/Mineral/Mineral.h"
#include "../Game/Building/BuildingMgr.h"

MineKuji::MineKuji()
{

	m_model.Load("Resource/Enemy/MineKuji/", "Minekuzi.gltf");
	m_attackedScale = 0.0f;
	m_scale = 0.0f;

}

void MineKuji::Init()
{

	m_isActive = false;
	m_isAttackWall = false;
	m_attackedScale = 0.0f;

}

void MineKuji::Generate(std::vector<KazMath::Vec3<float>> arg_route)
{

	m_route = arg_route;
	m_passedRouteIndex = 0;

	m_transform.pos = arg_route.front();
	m_scale = 0;
	m_isActive = true;
	m_isAttackedMineral = false;
	m_isAttackCore = false;
	m_isAttackMineral = false;
	m_mode = CoreAttack;
	m_attackID = ATTACK;
	m_gravity = 0.0f;
	m_attackedScale = 0.0f;
	m_oldTransform = m_transform;

	m_coreMoveSpeed = 0.0f;
	m_coreMoveDelayTimer = 0.0f;
	m_coreAttackDelayTimer = 0;
	m_coreAttackDelay = KazMath::Rand(MIN_CORE_ATTACK_DELAY, MAX_CORE_ATTACK_DELAY);

	m_hp = HP;
	m_attackedReactionVec = {};
	m_attackedMineral.reset();

	//���̐i�H�̕����ɐg�̂�������B
	KazMath::Vec3<float> forwardVec = KazMath::Vec3<float>(arg_route[1] - m_transform.pos).GetNormal();
	float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(forwardVec));
	KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(forwardVec);
	if (cross.Length() <= 0) {
		cross = { 0,1,0 };
	}
	DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);
	m_transform.quaternion = rotateQ;

	m_forwardVec = {0,0,1};
	m_isAttackWall = false;

}

void MineKuji::Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	//���W��ۑ����Ă����B
	m_oldTransform = m_transform;

	//���݂̏�Ԃɂ���ď����𕪂���B
	switch (m_mode)
	{
	case MineKuji::CoreAttack:
	{

		//�߂��ɃR�A�����邩����������B
		float coreDistance = KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos).Length();
		//�R�A�̋߂��ɋ��Ȃ�������R�A�̕����Ɍ������Ĉړ�������B
		if (CORE_ATTACK_RANGE < coreDistance) {

			//��莞�Ԓu���ɏ����x��^���ē������B
			m_coreMoveDelayTimer = std::clamp(m_coreMoveDelayTimer + 1.0f, 0.0f, CORE_MOVE_DELAY);
			if (CORE_MOVE_DELAY <= m_coreMoveDelayTimer) {

				m_coreMoveSpeed = CORE_MOVE_SPEED;
				m_coreMoveDelayTimer = 0.0f;

			}

			//�G�ɍU�����ꂽ�炷���Ɍ}����Ԃɓ���B
			if (m_isAttackedMineral) {

				m_mode = MineralAttack;

			}

			m_isAttackCore = false;

		}
		//�߂��ɃR�A����������U����ԂɈڂ�B
		else {

			AttackCore(arg_core);

			m_isAttackCore = true;

		}

		m_isAttackMineral = false;

	}
	break;
	case MineKuji::MineralAttack:
	{

		AttackMineral();

		m_isAttackMineral = true;

	}
	break;
	case MineKuji::WallAttack:
	{

		AttackWall();

	}
	break;
	default:
		break;
	}

	//�~�l�N�W���R�A�Ɍ������čs���ړ����v�Z����B
	if (0 < m_coreMoveSpeed) {

		Vec3<float> moveDir = SearchRoute();
		m_transform.pos += moveDir * m_coreMoveSpeed;
		m_coreMoveSpeed -= m_coreMoveSpeed / 15.0f;

	}
	else if (m_coreMoveSpeed < 0.01f) {
		m_coreMoveSpeed = 0.0f;
	}

	//�v���C���[�Ƃ̓����蔻��
	CheckHitPlayer(arg_player);

	//HP��0�ɂȂ����玀�S
	if (m_hp <= 0) {
		m_isActive = false;
	}

	//�X����B
	KazMath::Vec3<float> nowPos = m_transform.pos;
	KazMath::Vec3<float> viewTargetPos;
	nowPos.y = 0.0f;
	//�U�����̓^�[�Q�b�g�̕���������B
	if (m_isAttackCore) {

		nowPos = arg_core.lock()->GetPos();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;

	}
	else if (m_isAttackMineral || m_isAttackedMineral) {

		nowPos = m_attackedMineral.lock()->GetPos();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;

	}
	else {

		//�����������ɌX����B
		viewTargetPos = m_oldTransform.pos;
		viewTargetPos.y = 0.0f;

	}
	if (0 < KazMath::Vec3<float>(nowPos - viewTargetPos).Length()) {

		m_forwardVec = KazMath::Vec3<float>(nowPos - viewTargetPos).GetNormal();

		//�f�t�H���g�̐��ʃx�N�g������̉�]�ʂ����߂�B
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(m_forwardVec));

		//�N�H�[�^�j�I�������߂�B
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(m_forwardVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//��]��K��
		m_transform.quaternion = rotateQ;

	}

	//�U���̔�����^����B
	if (0.01f < m_coreAttackReactionVec.Length()) {

		m_transform.pos += m_coreAttackReactionVec;
		m_coreAttackReactionVec -= m_coreAttackReactionVec / 10.0f;

	}
	else {

		m_coreAttackReactionVec = {};

	}

	//�U�����󂯂��Ƃ��̃x�N�g����^����B
	if (0.01f < m_attackedReactionVec.Length()) {

		m_transform.pos += m_attackedReactionVec;
		m_attackedReactionVec -= m_attackedReactionVec / 10.0f;

	}
	else {

		m_attackedReactionVec = {};

	}

	//�󒆂ɕ����Ă�����d�͂��v�Z����B
	if (0 < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y -= m_gravity;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = 0.0f;

	}

	//�_���[�W���󂯂��Ƃ��̊g�k�����Ƃɖ߂��B
	if (0 < m_attackedScale) {

		m_attackedScale -= m_attackedScale / 5.0f;

	}
	else {

		m_attackedScale = 0.0f;

	}

	//�ǂƂ̓����蔻����s���B
	if (!m_isAttackWall) {
		MeshCollision::CheckHitResult result = BuildingMgr::Instance()->CheckHitWall(m_transform.pos, m_forwardVec, 40.0f, m_wallIndex);

		//�������Ă�����
		if (result.m_isHit) {

			m_isAttackWall = true;
			m_mode = WallAttack;

		}
	}

}

void MineKuji::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//�X�P�[�����f�t�H���g�̒l�ɋ߂Â���B
	if (m_isActive) {

		m_scale += (SCALE - m_scale) / 5.0f;

	}
	else {

		m_scale -= m_scale / 5.0f;
		if (m_scale < 0.1f) {
			m_scale = 0.0f;
		}

	}

	if (m_isActive) {
		m_transform.scale = { m_scale - m_attackedScale ,m_scale - m_attackedScale ,m_scale - m_attackedScale };
	}
	else {
		m_transform.scale = { m_scale ,m_scale ,m_scale };
	}

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

}

void MineKuji::Damage(std::weak_ptr<Mineral> arg_mineral, int arg_damage)
{

	m_isAttackedMineral = true;
	if (m_attackedMineral.expired()) {
		m_attackedMineral = arg_mineral;
	}
	m_hp = std::clamp(m_hp - arg_damage, 0, HP);

}

void MineKuji::AttackCore(std::weak_ptr<Core> arg_core)
{

	switch (m_attackID)
	{
	case MineKuji::ATTACK:
	{
		//�ړ����x���グ��B
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//�ړ�����x�N�g�������߂�B
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//�ړ����x�������𒴂��Ă�����͈͂Ɏ��߂�B
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//�ړ�������B
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//���������ȉ��ɂȂ�����ҋ@��Ԃ�
		if (KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos).Length() <= arg_core.lock()->GetScale() + m_transform.scale.x) {

			m_attackID = STAY;

			//�����Ő�����΂��B
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//�R�A�Ƀ_���[�W��^����B
			arg_core.lock()->Damage(1);

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case MineKuji::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		//�G�ɍU������Ă�����A�}����Ԃɓ���B
		if (m_isAttackedMineral) {

			m_mode = MineralAttack;

		}

		break;
	default:
		break;
	}

}

void MineKuji::AttackMineral()
{

	switch (m_attackID)
	{
	case MineKuji::ATTACK:
	{
		//�ړ����x���グ��B
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//�ړ�����x�N�g�������߂�B
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(m_attackedMineral.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//�ړ����x�������𒴂��Ă�����͈͂Ɏ��߂�B
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//�ړ�������B
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//���������ȉ��ɂȂ�����ҋ@��Ԃ�
		if (KazMath::Vec3<float>(m_attackedMineral.lock()->GetPosZeroY() - m_transform.pos).Length() <= m_attackedMineral.lock()->GetScale().x + m_transform.scale.x) {

			m_attackID = STAY;

			//�����Ő�����΂��B
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//�R�A�Ƀ_���[�W��^����B
			m_attackedMineral.lock()->Damage(1);

			if (m_attackedMineral.lock()->GetHP() <= 0) {
				m_attackedMineral.reset();
			}

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case MineKuji::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		//�G�ɍU������Ă�����A�}����Ԃɓ���B
		if (m_isAttackedMineral) {

			m_mode = MineralAttack;

		}

		break;
	default:
		break;
	}

}

void MineKuji::AttackWall()
{

	switch (m_attackID)
	{
	case MineKuji::ATTACK:
	{
		//�ړ����x���グ��B
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//�ړ�����x�N�g�������߂�B
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex) - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//�ړ����x�������𒴂��Ă�����͈͂Ɏ��߂�B
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//�ړ�������B
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//���������ȉ��ɂȂ�����ҋ@��Ԃ�
		if (KazMath::Vec3<float>(BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex) - m_transform.pos).Length() <= 5.0f + m_transform.scale.x) {

			m_attackID = STAY;

			//�����Ő�����΂��B
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//�R�A�Ƀ_���[�W��^����B
			BuildingMgr::Instance()->DamageWall(1);

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case MineKuji::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		//�G�ɍU������Ă�����A�}����Ԃɓ���B
		if (m_isAttackedMineral) {

			m_mode = MineralAttack;

		}

		break;
	default:
		break;
	}

	//�Q�Ƃ��Ă���ǂ���ꂽ��A�����B
	if (BuildingMgr::Instance()->GetWallHP(m_wallIndex) <= 0 || !BuildingMgr::Instance()->GetIsBuildWall(m_wallIndex)) {

		m_mode = CoreAttack;
		m_isAttackWall = false;

	}

}

void MineKuji::CheckHitPlayer(std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	//�v���C���[�Ƃ̓����蔻����s���B
	if (arg_player.lock()->GetIsDaipanTrigger()) {

		//�����蔻��`�F�b�N
		bool isHit = Vec3<float>(arg_player.lock()->GetTransform().pos - m_transform.pos).Length() <= arg_player.lock()->GetMineralAffectStrongRange() + 30.0f;
		if (isHit) {

			//�x�N�g���`�F�b�N�B
			KazMath::Vec3<float> dir = KazMath::Vec3<float>(m_transform.pos - arg_player.lock()->GetTransform().pos);
			dir.y = 0.0f;
			dir.Normalize();

			//HP�����炷�B
			m_hp = std::clamp(m_hp - 3, 0, HP);

			//�U���̔�����ǉ��B
			m_attackedReactionVec = dir / 5.0f;
			ShakeMgr::Instance()->m_shakeAmount = 2.0f;
			m_attackedScale = ATTACKED_SCALE;

		}

	}

}

KazMath::Vec3<float> MineKuji::SearchRoute()
{

	//���[�g�̍ő哞�B�n�_
	const int ROUTE_COUT = static_cast<int>(m_route.size());

	//���̃C���f�b�N�X���v�Z�B
	int nextIndex = m_passedRouteIndex + 1;

	//���̃C���f�b�N�X�����݂��邩�H
	if (ROUTE_COUT <= nextIndex) return {};

	//���̃C���f�b�N�X�ɓ��B���Ă�����B
	float nextIndexDistance = KazMath::Vec3<float>(m_route[nextIndex] - m_transform.pos).Length();
	if (nextIndexDistance < ROUTE_CHECK_SCALE) {

		//�C���f�b�N�X�����̂��̂ɂ���B
		m_passedRouteIndex = nextIndex;

		//���̃C���f�b�N�X�����邩���m�F�B
		nextIndex = m_passedRouteIndex + 1;

		//���̃C���f�b�N�X�����݂��邩�H
		if (ROUTE_COUT <= nextIndex) return {};

	}

	//�x�N�g�����v�Z�B
	KazMath::Vec3<float> moveDir = m_route[nextIndex] - m_transform.pos;
	moveDir.Normalize();

	return moveDir;

}
