#pragma once
#include "DrawCallSet.h"

class Player {

private:

	//プレイヤー描画関連
	DrawCallSet m_model;
	DrawCallSet m_attackModel;

	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_daipanPos;	//台パン中にカメラを固定するための座標
	KazMath::Vec3<float> m_forwardVec;			//正面ベクトル

	//隊列関連
	KazMath::Vec3<float> m_mineralCenterBasePos;	//ミネラルの中心座標。隊列を移動させる時に使う。
	KazMath::Vec3<float> m_mineralCenterPos;		//ミネラルの中心座標。隊列を移動させる時に使う。
	const float MINERAL_CENTER_DISTANCE = 10.0f;
	const float MINERAL_MOVE_SPEED = 0.8f;

	//台パンの範囲のモデル関連
	DrawCallSet m_daipanModel;
	DrawCallSet m_daipanStrongModel;

	//台パン関連
	bool m_isInputDaipan;	//台パンのキーが入力されているか
	bool m_isOldInputDaipan;//台パンのキーの入力の前フレーム
	enum DAIPAN_STATE {
		NONE,
		CHARGE,			//入力をチャージしている間。
		DAIPAN_NOW,		//台パン中

	}m_daipanStatus;

	//強台パンの変数
	bool m_isDaipanStrong;
	float m_daipanStrongTimer;
	const float DAIPAN_STRONG_TIMER = 40.0f;
	KazMath::Vec3<float> m_daipanShakePos;
	const float DAIPAN_SHAKE_POS = 0.5f;

	//台パン中の変数
	float m_daipanStartPosY;
	float m_daipanTimer;
	const float DAIPAN_NOW_TIMER = 7.0f;
	const float DAIPAN_NOW_STRONG_TIMER = 4.0f;
	const float DAIPAN_SHAKE = 1.0f;
	const float DAIPAN_STRONG_SHAKE = 1.8f;

	//台パン終了後に戻るタイマー
	float m_daipanReturnTimer;
	const float DAIPAN_RETURN_TIMER = 10.0f;

	//Y軸関連
	const float DEFAULT_Y = 30.0f;
	const float DAIPAN_Y = DEFAULT_Y + 10.0f;

	//ミネラルを集合させる範囲
	const float MINERAL_AFFECT_RANGE = 40.0f;
	const float MINERAL_AFFECT_STRONG_RANGE = 10.0f;

	//ミネラル解散関連。
	bool m_isBreakUp;

public:

	Player();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//座標を取得する。
	KazMath::Transform3D GetTransform() { return m_transform; }
	//Y軸を0にした座標を取得する。
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	//座標を取得する。
	KazMath::Vec3<float> GetMineralCenterPos() { return m_mineralCenterBasePos + m_mineralCenterPos; }
	//正面ベクトルを持ってくる、。
	KazMath::Vec3<float> GetForwardVec() { return m_forwardVec; }
	//Y軸をデフォルトの高さにした現在座標を取得する。
	KazMath::Vec3<float> GetTransformDefaultY() { return KazMath::Vec3<float>(m_daipanPos.x, DEFAULT_Y, m_daipanPos.z); }
	//ミネラルに影響を与える範囲。
	float GetMineralAffectRange() { return MINERAL_AFFECT_RANGE; }
	float GetMineralAffectStrongRange() { return MINERAL_AFFECT_STRONG_RANGE; }
	//台パンのタイミングか
	bool GetIsDaipanTrigger() { return (m_isDaipanStrong ? DAIPAN_NOW_STRONG_TIMER : DAIPAN_NOW_TIMER) - 1.0f == m_daipanTimer; }
	//強台パンか？
	bool GetIsStrongDaipan() { return m_isDaipanStrong; }
	//解散状態か？
	bool GetIsBreakUP() { return m_isBreakUp; }

};