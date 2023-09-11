#pragma once
#include "DrawCallSet.h"

class Player {

private:

	//�v���C���[�`��֘A
	DrawCallSet m_model;
	DrawCallSet m_attackModel;

	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_daipanPos;	//��p�����ɃJ�������Œ肷�邽�߂̍��W
	KazMath::Vec3<float> m_forwardVec;			//���ʃx�N�g��

	//����֘A
	KazMath::Vec3<float> m_mineralCenterBasePos;	//�~�l�����̒��S���W�B������ړ������鎞�Ɏg���B
	KazMath::Vec3<float> m_mineralCenterPos;		//�~�l�����̒��S���W�B������ړ������鎞�Ɏg���B
	const float MINERAL_CENTER_DISTANCE = 10.0f;
	const float MINERAL_MOVE_SPEED = 0.8f;

	//��p���͈̔͂̃��f���֘A
	DrawCallSet m_daipanModel;
	DrawCallSet m_daipanStrongModel;

	//��p���֘A
	bool m_isInputDaipan;	//��p���̃L�[�����͂���Ă��邩
	bool m_isOldInputDaipan;//��p���̃L�[�̓��͂̑O�t���[��
	enum DAIPAN_STATE {
		NONE,
		CHARGE,			//���͂��`���[�W���Ă���ԁB
		DAIPAN_NOW,		//��p����

	}m_daipanStatus;

	//����p���̕ϐ�
	bool m_isDaipanStrong;
	float m_daipanStrongTimer;
	const float DAIPAN_STRONG_TIMER = 40.0f;
	KazMath::Vec3<float> m_daipanShakePos;
	const float DAIPAN_SHAKE_POS = 0.5f;

	//��p�����̕ϐ�
	float m_daipanStartPosY;
	float m_daipanTimer;
	const float DAIPAN_NOW_TIMER = 7.0f;
	const float DAIPAN_NOW_STRONG_TIMER = 4.0f;
	const float DAIPAN_SHAKE = 1.0f;
	const float DAIPAN_STRONG_SHAKE = 1.8f;

	//��p���I����ɖ߂�^�C�}�[
	float m_daipanReturnTimer;
	const float DAIPAN_RETURN_TIMER = 10.0f;

	//Y���֘A
	const float DEFAULT_Y = 30.0f;
	const float DAIPAN_Y = DEFAULT_Y + 10.0f;

	//�~�l�������W��������͈�
	const float MINERAL_AFFECT_RANGE = 40.0f;
	const float MINERAL_AFFECT_STRONG_RANGE = 10.0f;

	//�~�l�������U�֘A�B
	bool m_isBreakUp;

public:

	Player();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//���W���擾����B
	KazMath::Transform3D GetTransform() { return m_transform; }
	//Y����0�ɂ������W���擾����B
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	//���W���擾����B
	KazMath::Vec3<float> GetMineralCenterPos() { return m_mineralCenterBasePos + m_mineralCenterPos; }
	//���ʃx�N�g���������Ă���A�B
	KazMath::Vec3<float> GetForwardVec() { return m_forwardVec; }
	//Y�����f�t�H���g�̍����ɂ������ݍ��W���擾����B
	KazMath::Vec3<float> GetTransformDefaultY() { return KazMath::Vec3<float>(m_daipanPos.x, DEFAULT_Y, m_daipanPos.z); }
	//�~�l�����ɉe����^����͈́B
	float GetMineralAffectRange() { return MINERAL_AFFECT_RANGE; }
	float GetMineralAffectStrongRange() { return MINERAL_AFFECT_STRONG_RANGE; }
	//��p���̃^�C�~���O��
	bool GetIsDaipanTrigger() { return (m_isDaipanStrong ? DAIPAN_NOW_STRONG_TIMER : DAIPAN_NOW_TIMER) - 1.0f == m_daipanTimer; }
	//����p�����H
	bool GetIsStrongDaipan() { return m_isDaipanStrong; }
	//���U��Ԃ��H
	bool GetIsBreakUP() { return m_isBreakUp; }

};