#pragma once
#include "../Game/DrawCallSet.h"

class Core {

private:

	DrawCallSet m_model;
	DrawCallSet m_crownModel;
	KazMath::Vec3<float> m_crownPos;
	KazMath::Transform3D m_transform;
	int m_hp;
	const int MAX_HP = 20;
	const float SCALE = 10.0f;

public:

	Core();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void Damage(int arg_damage) { m_hp = std::clamp(m_hp - arg_damage, 0, MAX_HP); }

	void SetPos(KazMath::Vec3<float> arg_pos) { m_transform.pos = arg_pos; };
	float GetScale() { return SCALE; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return { m_transform.pos.x, 0.0f, m_transform.pos.z }; }

};