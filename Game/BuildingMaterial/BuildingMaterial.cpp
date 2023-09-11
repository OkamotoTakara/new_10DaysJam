#include "BuildingMaterial.h"

BuildingMaterial::BuildingMaterial()
{

	m_model.Load("Resource/Mineral/", "Material_tree_01.gltf");
	m_isActive = false;
	m_isHeld = false;
	m_gravity = 0.0f;

}

void BuildingMaterial::Init()
{

	m_isActive = false;
	m_isHeld = false;
	m_gravity = 0.0f;

}

void BuildingMaterial::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_generateVec)
{

	m_transform.pos = arg_pos;
	m_generateVec = arg_generateVec;
	m_isActive = true;
	m_isHeld = false;
	m_gravity = 0.0f;

}

void BuildingMaterial::Update()
{

	if (m_isHeld) return;

	//座標を更新。
	if (0.01f < m_generateVec.Length()) {

		m_transform.pos += m_generateVec;
		m_generateVec -= m_generateVec / 10.0f;

	}
	else {

		m_generateVec = {};

	}

	//重力をかける。
	if (0 < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y =- m_gravity;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = 0.0f;

	}

}

void BuildingMaterial::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (m_isActive) {

		m_transform.scale = { SCALE,SCALE,SCALE };

	}
	else {

		m_transform.scale = { 0,0,0 };

	}


	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

}
