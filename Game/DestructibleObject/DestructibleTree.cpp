#include "DestructibleTree.h"
#include "../Game/BuildingMaterial/BuildingMaterialMgr.h"

DestructibleTree::DestructibleTree()
{

	m_model.Load("Resource/DestructibleObject/Tree/", "Tree.gltf");
	m_transform.scale = { 5,5,5 };
	m_isActive = false;
	m_hp = HP;

}

void DestructibleTree::Init()
{

	m_isActive = false;
	m_hp = HP;

}

void DestructibleTree::Generate(KazMath::Vec3<float> arg_generatePos)
{

	m_isActive = true;
	m_transform.pos = arg_generatePos;
	m_hp = HP;

}

void DestructibleTree::Update()
{

	if (m_hp <= 0) {

		m_isActive = false;
		
		//‘fÞ‚ðŽU‚ç‚Î‚ç‚¹‚éB
		const int MATERIAL_COUNT = 3;
		for (int index = 0; index < 3; ++index) {
			const float STRONG_DAIPAN_POWER = 2.0f;
			KazMath::Vec3<float> generateVec;
			generateVec = { KazMath::Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
			generateVec.GetNormal();
			generateVec *= STRONG_DAIPAN_POWER;
			BuildingMaterialMgr::Instance()->Generate(m_transform.pos, generateVec);
		}

	}

}

void DestructibleTree::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (m_isActive) {
		m_transform.scale = { 5,5,5 };
	}
	else {
		m_transform.scale = { 0,0,0 };
	}

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

}

void DestructibleTree::Damage(int arg_damage) {

	m_hp = std::clamp(m_hp - arg_damage, 0, HP);

}