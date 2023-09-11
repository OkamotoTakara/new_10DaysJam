#include "Core.h"

Core::Core()
{

	m_model.LoadOutline("Resource/Core/", "Core.gltf");
	m_crownModel.LoadOutline("Resource/Core/", "Crown.gltf");
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




	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

	outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	m_crownModel.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_crownModel.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_crownModel.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_crownModel.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	KazMath::Transform3D crownTransform = m_transform;
	crownTransform.pos += m_crownPos;
	m_crownModel.Draw(arg_rasterize, arg_blasVec, crownTransform);

}
