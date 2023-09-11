#include "Player.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/Effect/ShakeMgr.h"
#include "../KazLibrary/Input/ControllerInputManager.h"
#include "../Game/Collision/StageCollision.h"

Player::Player()
{

	m_model.Load("Resource/Player/", "Hand.gltf", true);

	m_attackModel.Load("Resource/Player/", "HandAttack.gltf", true);

	m_daipanModel.Load("Resource/Player/HandCircle/", "HandCircle.gltf");

	m_daipanStrongModel.Load("Resource/Player/HandCircle/", "HandCircleStrong.gltf");

}

void Player::Init()
{

	m_isInputDaipan = false;
	m_isDaipanStrong = false;
	m_isBreakUp = false;
	m_daipanStatus = NONE;
	m_daipanStartPosY = 0;
	m_daipanTimer = 0;
	m_daipanReturnTimer = 0;
	m_daipanStrongTimer = 0.0f;
	m_daipanShakePos = {};
	m_transform.pos.y = DEFAULT_Y;
	m_daipanPos = m_transform.pos;
	m_mineralCenterPos = m_transform.pos;
	m_mineralCenterPos.y = 0.0f;
	m_oldTransform = m_transform;

}

void Player::Update()
{

	//シェイクをもとに戻す。
	m_transform.pos -= m_daipanShakePos;

	//座標を保存しておく。
	m_oldTransform = m_transform;

	//プレイヤーを動かす。
	const float MOVE_SPEED = 1.0f;
	m_transform.pos.z += (KeyBoradInputManager::Instance()->InputState(DIK_W)) * MOVE_SPEED;
	m_transform.pos.z -= (KeyBoradInputManager::Instance()->InputState(DIK_S)) * MOVE_SPEED;
	m_transform.pos.x += (KeyBoradInputManager::Instance()->InputState(DIK_D)) * MOVE_SPEED;
	m_transform.pos.x -= (KeyBoradInputManager::Instance()->InputState(DIK_A)) * MOVE_SPEED;

	//コントローラーも対応。
	float inputStickX = ControllerInputManager::Instance()->GetJoyStickLXNum() / 32767.0f;
	float inputStickY = ControllerInputManager::Instance()->GetJoyStickLYNum() / 32767.0f;
	const float STICK_DEADLINE = 0.3f;
	if (STICK_DEADLINE <= fabs(inputStickX)) {
		m_transform.pos.x += inputStickX * MOVE_SPEED;
	}
	if (STICK_DEADLINE <= fabs(inputStickY)) {
		m_transform.pos.z += inputStickY * MOVE_SPEED;
	}

	//隊列を操作する。
	m_mineralCenterPos.z += (KeyBoradInputManager::Instance()->InputState(DIK_UP)) * MINERAL_MOVE_SPEED;
	m_mineralCenterPos.z -= (KeyBoradInputManager::Instance()->InputState(DIK_DOWN)) * MINERAL_MOVE_SPEED;
	m_mineralCenterPos.x += (KeyBoradInputManager::Instance()->InputState(DIK_RIGHT)) * MINERAL_MOVE_SPEED;
	m_mineralCenterPos.x -= (KeyBoradInputManager::Instance()->InputState(DIK_LEFT)) * MINERAL_MOVE_SPEED;
	inputStickX = ControllerInputManager::Instance()->GetJoyStickRXNum() / 32767.0f;
	inputStickY = ControllerInputManager::Instance()->GetJoyStickRYNum() / 32767.0f;
	if (STICK_DEADLINE <= fabs(inputStickX)) {
		m_mineralCenterPos.x += inputStickX * MINERAL_MOVE_SPEED;
	}
	if (STICK_DEADLINE <= fabs(inputStickY)) {
		m_mineralCenterPos.z += inputStickY * MINERAL_MOVE_SPEED;
	}

	//隊列の操作量に上限を設ける。
	const float MINERAL_CENTER_MOVE_RANGE = 10.0f;
	if (MINERAL_CENTER_MOVE_RANGE < m_mineralCenterPos.Length()) {
		m_mineralCenterPos = m_mineralCenterPos.GetNormal() * MINERAL_CENTER_MOVE_RANGE;
	}

	//台パンが入力されているか。
	m_isOldInputDaipan = m_isInputDaipan;
	m_isInputDaipan = KeyBoradInputManager::Instance()->InputState(DIK_SPACE) || ControllerInputManager::Instance()->InputState(XINPUT_GAMEPAD_A);

	//解散状態かを更新
	m_isBreakUp = KeyBoradInputManager::Instance()->InputState(DIK_T) || ControllerInputManager::Instance()->InputState(XINPUT_GAMEPAD_B);

	//台パン中はカメラを追尾させない仕様を切るためのやつ。確認を取ったら正式に実装する。
	m_daipanPos += (m_transform.pos - m_daipanPos) / 3.0f;

	//ステータスによって台パンの状態を変える。
	switch (m_daipanStatus)
	{
	case Player::NONE:

		//入力があったらステータスを変える。
		if (m_isInputDaipan) {

			m_daipanStatus = Player::CHARGE;

		}

		//座標をデフォルトの位置に補間する。
		m_transform.pos.y += (DEFAULT_Y - m_transform.pos.y) / 10.0f;

		//なにもない時のみ台パン座標を保存する。
		//m_daipanPos += (m_transform.pos - m_daipanPos) / 3.0f;

		//何もしないときは初期化する。
		m_daipanTimer = 0.0f;
		m_daipanReturnTimer = 0.0f;
		m_daipanStartPosY = 0.0f;
		m_daipanStrongTimer = 0.0f;
		m_daipanShakePos = {};

		break;
	case Player::CHARGE:

	{

		//高さをチャージ中の位置に動かしていく。
		m_transform.pos.y += (DAIPAN_Y - m_transform.pos.y) / 10.0f;

		//リリースされた瞬間だったら
		if (m_isOldInputDaipan && !m_isInputDaipan) {

			m_daipanStatus = Player::DAIPAN_NOW;
			m_daipanStartPosY = m_transform.pos.y;

		}

		//台パンを強にするかのタイマーを更新。
		m_daipanStrongTimer = std::clamp(m_daipanStrongTimer + 1.0f, 0.0f, DAIPAN_STRONG_TIMER);
		m_isDaipanStrong = DAIPAN_STRONG_TIMER <= m_daipanStrongTimer;

		//台パン中はシェイクさせる。
		if (m_isDaipanStrong) {

			//シェイク量を計算
			m_daipanShakePos = { KazMath::Rand(-DAIPAN_SHAKE_POS, DAIPAN_SHAKE_POS),KazMath::Rand(-DAIPAN_SHAKE_POS, DAIPAN_SHAKE_POS),KazMath::Rand(-DAIPAN_SHAKE_POS, DAIPAN_SHAKE_POS) };

		}

	}

	break;
	case Player::DAIPAN_NOW:
	{

		//台パンで振り下ろすやつのタイマーを設定する。
		float daipanTimer = DAIPAN_NOW_TIMER;
		if (m_isDaipanStrong) {
			daipanTimer = DAIPAN_NOW_STRONG_TIMER;
		}

		//キャンセルのタイマーを更新。
		m_daipanTimer = std::clamp(m_daipanTimer + 1.0f, 0.0f, daipanTimer);

		//Y軸をもとに戻す。
		float easingAmount = EasingMaker(In, Cubic, m_daipanTimer / daipanTimer);

		//Y軸を設定。
		m_transform.pos.y = m_daipanStartPosY - m_daipanStartPosY * easingAmount;

		//タイマーが上限に達したら通常状態に戻す。
		if (daipanTimer <= m_daipanTimer) {

			m_daipanStatus = Player::NONE;

			//強大パンだったらそれ用のシェイクにする。
			ShakeMgr::Instance()->m_shakeAmount = m_isDaipanStrong ? DAIPAN_STRONG_SHAKE : DAIPAN_SHAKE;

		}

	}

	break;

	default:
		break;
	}

	//本来あるべきミネラルの中心点を計算する。
	m_mineralCenterBasePos = m_transform.pos;
	if (!m_isDaipanStrong) {

		m_mineralCenterBasePos -= m_forwardVec * MINERAL_CENTER_DISTANCE;

	}
	m_mineralCenterBasePos.y = 0.0f;

	//動いた方向に傾ける。
	KazMath::Vec3<float> nowPos = m_transform.pos;
	nowPos.y = 0.0f;
	KazMath::Vec3<float> oldPos = m_oldTransform.pos;
	oldPos.y = 0.0f;
	if (0 < KazMath::Vec3<float>(nowPos - oldPos).Length()) {

		m_forwardVec = KazMath::Vec3<float>(nowPos - oldPos).GetNormal();

		//デフォルトの正面ベクトルからの回転量を求める。
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(m_forwardVec));

		//クォータニオンを求める。
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(m_forwardVec);
		if (cross.Length() <= 0) {
			cross = {0,1,0};
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//回転を適応
		m_transform.quaternion = rotateQ;

	}

	//当たり判定を計算。
	const float RAY_LENGTH = 10.0f;
	MeshCollision::CheckHitResult rayResult = StageCollision::Instance()->m_stageCollision.CheckHitRay(m_transform.pos, m_forwardVec);
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}
	
	//シェイクをかける。
	m_transform.pos += m_daipanShakePos;

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//プレイヤー本体を描画
	m_transform.scale = { 9.0f, 9.0f, 9.0f };
	if (m_isDaipanStrong && m_daipanStatus != NONE) {
		m_attackModel.Draw(arg_rasterize, arg_blasVec, m_transform, 10);
	}
	else {
		m_model.Draw(arg_rasterize, arg_blasVec, m_transform, 10);
	}

	//プレイヤーの台パンの範囲を描画
	KazMath::Transform3D transform;
	transform.scale = { 0,0,0 };
	if (m_isInputDaipan && !m_isDaipanStrong) {

		transform.pos = m_transform.pos;
		transform.pos.y = 0.0f;
		transform.scale = { MINERAL_AFFECT_RANGE,MINERAL_AFFECT_RANGE,MINERAL_AFFECT_RANGE };

	}
	m_daipanModel.Draw(arg_rasterize, arg_blasVec, transform);

	//プレイヤーの強台パンの範囲を描画する。
	transform.scale = { 0,0,0 };
	if (m_isInputDaipan && m_isDaipanStrong) {

		transform.pos = m_transform.pos;
		transform.pos.y = 0.0f;
		transform.scale = { MINERAL_AFFECT_STRONG_RANGE,MINERAL_AFFECT_STRONG_RANGE + 1.0f,MINERAL_AFFECT_STRONG_RANGE };

	}
	m_daipanStrongModel.Draw(arg_rasterize, arg_blasVec, transform);

}
