#include "MineKuji.h"
#include "../Game/Core/Core.h"
#include "../Effect/ShakeMgr.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Game/Player.h"
#include "../Game/Mineral/Mineral.h"
#include "../Game/Building/BuildingMgr.h"
#include "../Game/Mineral/MineralMgr.h"

MineKuji::MineKuji()
{

	m_model.LoadOutline("Resource/Enemy/MineKuji/", "Minekuzi.gltf");
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

	//次の進路の方向に身体を向ける。
	KazMath::Vec3<float> forwardVec = KazMath::Vec3<float>(arg_route[1] - m_transform.pos).GetNormal();
	float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(forwardVec));
	KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(forwardVec);
	if (cross.Length() <= 0) {
		cross = { 0,1,0 };
	}
	DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);
	m_transform.quaternion = rotateQ;

	m_forwardVec = { 0,0,1 };
	m_isAttackWall = false;

}

void MineKuji::Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	//座標を保存しておく。
	m_oldTransform = m_transform;

	//現在の状態によって処理を分ける。
	switch (m_mode)
	{
	case MineKuji::CoreAttack:
	{

		//近くにコアがあるかを検索する。
		float coreDistance = KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos).Length();
		//コアの近くに居なかったらコアの方向に向かって移動させる。
		if (CORE_ATTACK_RANGE < coreDistance) {

			//一定時間置きに初速度を与えて動かす。
			m_coreMoveDelayTimer = std::clamp(m_coreMoveDelayTimer + 1.0f, 0.0f, CORE_MOVE_DELAY);
			if (CORE_MOVE_DELAY <= m_coreMoveDelayTimer) {

				m_coreMoveSpeed = CORE_MOVE_SPEED;
				m_coreMoveDelayTimer = 0.0f;

			}

			//近くにミネラルが居るか？
			int mineralIndex = 0;
			if (MineralMgr::Instance()->SearchNearMineral(GetPosZeroY(), ENEMY_SEARCH_RANGE, mineralIndex)) {

				m_mode = MineralAttack;
				m_isAttackedMineral = true;
				m_isAttackMineral = false;
				m_isAttackWall = false;
				m_isAttackPlayer = false;
				m_attackedMineral = MineralMgr::Instance()->GetMineral(mineralIndex);
				break;

			}

			//近くにプレイヤーが居るか？
			if (!arg_player.lock()->GetIsStun() && KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length() <= ENEMY_SEARCH_RANGE) {


				m_mode = PlayerAttack;
				m_isAttackedMineral = false;
				m_isAttackMineral = false;
				m_isAttackWall = false;
				m_isAttackPlayer = true;
				break;

			}

			//敵に攻撃されたらすぐに迎撃状態に入る。
			if (m_isAttackedMineral) {

				m_mode = MineralAttack;

			}

			m_isAttackCore = false;

		}
		//近くにコアがあったら攻撃状態に移る。
		else {

			AttackCore(arg_core);

			m_isAttackCore = true;

		}

		m_isAttackMineral = false;
		m_isAttackWall = false;
		m_isAttackPlayer = false;

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
	case MineKuji::PlayerAttack:
	{

		AttackPlayer(arg_player);

	}
	break;
	default:
		break;
	}

	//動かす。
	Move();

	//当たり判定
	CheckHit(arg_player);

	//回転させる。
	Rotation(arg_core, arg_player);

	//HPが0になったら死亡
	if (m_hp <= 0) {
		m_isActive = false;
	}

}

void MineKuji::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//スケールをデフォルトの値に近づける。
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



	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.2f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

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

void MineKuji::CounterOverlap(KazMath::Vec3<float> arg_centerPos, KazMath::Vec3<float> arg_mineralPos, float arg_scale)
{

	//重なっているかをチェックする。
	float mineralLength = KazMath::Vec3<float>(arg_mineralPos - GetPosZeroY()).Length();
	bool isHit = mineralLength <= m_scale * 2.0f + arg_scale;

	//重なっていなかったら問題ない。
	if (!isHit) return;

	//当たっていたらどっちが外側に居るかを判断して、外側に居る方を押し戻す。
	float thisMineralLength = KazMath::Vec3<float>(arg_centerPos - GetPosZeroY()).Length();
	float argMineralLength = KazMath::Vec3<float>(arg_centerPos - arg_mineralPos).Length();
	bool isOutsideThisMineral = argMineralLength < thisMineralLength;

	//押し戻す量を計算。
	float pushBackLength = (arg_scale + m_scale * 2.0f) - mineralLength;

	//このミネラルが外側だったら
	if (isOutsideThisMineral) {

		//押し戻すベクトルを計算。
		KazMath::Vec3<float> pushBackVec = KazMath::Vec3<float>(GetPosZeroY() - arg_mineralPos);
		pushBackVec.y = 0.0f;
		pushBackVec.Normalize();

		//押し戻す。
		m_transform.pos += pushBackVec * pushBackLength;


	}
	else {

		//押し戻すベクトルを計算。
		KazMath::Vec3<float> pushBackVec = KazMath::Vec3<float>(arg_mineralPos - GetPosZeroY());
		pushBackVec.y = 0.0f;
		pushBackVec.Normalize();

		//押し戻す。
		arg_mineralPos += pushBackVec * pushBackLength;

	}

}

void MineKuji::AttackCore(std::weak_ptr<Core> arg_core)
{

	switch (m_attackID)
	{
	case MineKuji::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		if (KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos).Length() <= arg_core.lock()->GetScale() + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//コアにダメージを与える。
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

		//敵に攻撃されていたら、迎撃状態に入る。
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

	//参照が切れていたら終わり。
	if (m_attackedMineral.expired()) {

		m_mode = CoreAttack;
		m_isAttackMineral = false;
		return;
	}

	//攻撃対象のミネラルが一定以上離れていたらコア攻撃状態二遷移。
	float mineralDistance = KazMath::Vec3<float>(GetPosZeroY() - m_attackedMineral.lock()->GetPosZeroY()).Length();
	if (ENEMY_SEARCH_END_RANGE < mineralDistance) {

		m_mode = CoreAttack;
		m_isAttackMineral = false;
		m_attackedMineral.reset();
		return;

	}

	//この変数が初期枯れてていない場合があるので、ミネラルを攻撃しているときは壁を攻撃していないだろってことなのでこれ。
	m_isAttackWall = false;

	switch (m_attackID)
	{
	case MineKuji::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(m_attackedMineral.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		if (KazMath::Vec3<float>(m_attackedMineral.lock()->GetPosZeroY() - m_transform.pos).Length() <= m_attackedMineral.lock()->GetScale().x + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 1.5f);

			//コアにダメージを与える。
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

		//敵に攻撃されていたら、迎撃状態に入る。
		if (m_isAttackedMineral) {

			m_mode = MineralAttack;

		}

		break;
	default:
		break;
	}

}

void MineKuji::AttackPlayer(std::weak_ptr<Player> arg_player)
{

	//参照が切れていたら終わり。
	if (arg_player.lock()->GetIsStun()) {

		m_mode = CoreAttack;
		m_isAttackPlayer = false;
		return;
	}

	//攻撃対象のミネラルが一定以上離れていたらコア攻撃状態二遷移。
	float mineralDistance = KazMath::Vec3<float>(GetPosZeroY() - arg_player.lock()->GetPosZeroY()).Length();
	if (ENEMY_SEARCH_END_RANGE < mineralDistance) {

		m_mode = CoreAttack;
		m_isAttackPlayer = false;
		return;

	}

	//この変数が初期枯れてていない場合があるので、ミネラルを攻撃しているときは壁を攻撃していないだろってことなのでこれ。
	m_isAttackWall = false;

	switch (m_attackID)
	{
	case MineKuji::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		distance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - m_transform.pos).Length();
		if (distance <= arg_player.lock()->GetHitScale() + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 1.5f);

			//コアにダメージを与える。
			arg_player.lock()->Damage(1);


			ShakeMgr::Instance()->m_shakeAmount = 3.0;

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
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex) - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		if (KazMath::Vec3<float>(BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex) - m_transform.pos).Length() <= 5.0f + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//コアにダメージを与える。
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

		break;
	default:
		break;
	}

	//参照している壁が壊れたら、おわり。
	if (BuildingMgr::Instance()->GetWallHP(m_wallIndex) <= 0 || !BuildingMgr::Instance()->GetIsBuildWall(m_wallIndex)) {

		m_mode = CoreAttack;
		m_isAttackWall = false;

	}

	m_isAttackMineral = false;
	m_coreMoveSpeed = 0.0f;

}

void MineKuji::CheckHitPlayer(std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	//プレイヤーとの当たり判定を行う。
	if (arg_player.lock()->GetIsDaipanTrigger()) {

		//当たり判定チェック
		bool isHit = Vec3<float>(arg_player.lock()->GetTransform().pos - m_transform.pos).Length() <= arg_player.lock()->GetMineralAffectStrongRange() + 30.0f;
		if (isHit) {

			//ベクトルチェック。
			KazMath::Vec3<float> dir = KazMath::Vec3<float>(m_transform.pos - arg_player.lock()->GetTransform().pos);
			dir.y = 0.0f;
			dir.Normalize();

			//HPを減らす。
			m_hp = std::clamp(m_hp - 3, 0, HP);

			//攻撃の反動を追加。
			m_attackedReactionVec = dir / 5.0f;
			ShakeMgr::Instance()->m_shakeAmount = 2.0f;
			m_attackedScale = ATTACKED_SCALE;

		}

	}

}

KazMath::Vec3<float> MineKuji::SearchRoute()
{

	//ルートの最大到達地点
	const int ROUTE_COUT = static_cast<int>(m_route.size());

	//次のインデックスを計算。
	int nextIndex = m_passedRouteIndex + 1;

	//次のインデックスが存在するか？
	if (ROUTE_COUT <= nextIndex) return {};

	//次のインデックスに到達していたら。
	float nextIndexDistance = KazMath::Vec3<float>(m_route[nextIndex] - m_transform.pos).Length();
	if (nextIndexDistance < ROUTE_CHECK_SCALE) {

		//インデックスを次のものにする。
		m_passedRouteIndex = nextIndex;

		//次のインデックスがあるかを確認。
		nextIndex = m_passedRouteIndex + 1;

		//次のインデックスが存在するか？
		if (ROUTE_COUT <= nextIndex) return {};

	}

	//ベクトルを計算。
	KazMath::Vec3<float> moveDir = m_route[nextIndex] - m_transform.pos;
	moveDir.Normalize();

	return moveDir;

}

void MineKuji::Move() {

	using namespace KazMath;

	//ミネクジがコアに向かって行く移動を計算する。
	if (0 < m_coreMoveSpeed) {

		Vec3<float> moveDir = SearchRoute();
		m_transform.pos += moveDir * m_coreMoveSpeed;
		m_coreMoveSpeed -= m_coreMoveSpeed / 15.0f;

	}
	else if (m_coreMoveSpeed < 0.01f) {
		m_coreMoveSpeed = 0.0f;
	}

	//攻撃の反動を与える。
	if (0.01f < m_coreAttackReactionVec.Length()) {

		m_transform.pos += m_coreAttackReactionVec;
		m_coreAttackReactionVec -= m_coreAttackReactionVec / 10.0f;

	}
	else {

		m_coreAttackReactionVec = {};

	}

	//攻撃を受けたときのベクトルを与える。
	if (0.01f < m_attackedReactionVec.Length()) {

		m_transform.pos += m_attackedReactionVec;
		m_attackedReactionVec -= m_attackedReactionVec / 10.0f;

	}
	else {

		m_attackedReactionVec = {};

	}

	//空中に浮いていたら重力を計算する。
	const float UNDER_Y = 5.0f;
	if (UNDER_Y < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y -= m_gravity;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = UNDER_Y;

	}

	//ダメージを受けたときの拡縮をもとに戻す。
	if (0 < m_attackedScale) {

		m_attackedScale -= m_attackedScale / 5.0f;

	}
	else {

		m_attackedScale = 0.0f;

	}

}


void MineKuji::CheckHit(std::weak_ptr<Player> arg_player) {


	//プレイヤーとの当たり判定
	CheckHitPlayer(arg_player);

	//壁との当たり判定を行う。
	MeshCollision::CheckHitResult result = BuildingMgr::Instance()->CheckHitWall(m_transform.pos, m_forwardVec, 40.0f, m_wallIndex);

	//当たっていたら
	if (result.m_isHit) {

		m_isAttackWall = true;
		m_mode = WallAttack;

	}

}

void MineKuji::Rotation(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player) {



	//傾ける。
	KazMath::Vec3<float> nowPos = m_transform.pos;
	KazMath::Vec3<float> viewTargetPos;
	nowPos.y = 0.0f;
	//攻撃中はターゲットの方向を見る。
	if (m_isAttackCore) {

		nowPos = arg_core.lock()->GetPos();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;

	}
	else if (m_isAttackWall) {
		nowPos = BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex);
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;
	}
	else if (m_isAttackPlayer) {

		nowPos = arg_player.lock()->GetPosZeroY();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;
	}
	else if ((m_isAttackMineral || m_isAttackedMineral) && !m_attackedMineral.expired()) {

		nowPos = m_attackedMineral.lock()->GetPos();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;

	}
	else {

		//動いた方向に傾ける。
		viewTargetPos = m_oldTransform.pos;
		viewTargetPos.y = 0.0f;

	}
	if (0 < KazMath::Vec3<float>(nowPos - viewTargetPos).Length()) {

		m_forwardVec = KazMath::Vec3<float>(nowPos - viewTargetPos).GetNormal();

		//デフォルトの正面ベクトルからの回転量を求める。
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(m_forwardVec));

		//クォータニオンを求める。
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(m_forwardVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//回転を適応
		m_transform.quaternion = rotateQ;

	}

}