#include "Core.h"

Core::Core()
{

	m_model.Load("Resource/Core/", "Core.gltf");
	m_crownModel.Load("Resource/Core/", "Crown.gltf");
	m_crownPos = KazMath::Vec3<float>(0.6f, 17.6f, -5.2f);
	m_hp = MAX_HP;

}

void Core::Init()
{

	m_hp = MAX_HP;

}

void Core::Update()
{

	m_transform.scale = { SCALE ,SCALE ,SCALE };
	m_transform.rotation.y = 270.0f;

}

void Core::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

	KazMath::Transform3D crownTransform = m_transform;
	crownTransform.pos += m_crownPos;
	m_crownModel.Draw(arg_rasterize, arg_blasVec, crownTransform);

}
