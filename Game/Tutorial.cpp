#include "Tutorial.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include"../Game/TitleFlag.h"

void Tutorial::setting()
{
	//テクスチャのロード
	tutorial_tex[0].Load("Resource/TutorialTex/UI_Tutorial1.png");
	tutorial_tex[1].Load("Resource/TutorialTex/UI_Tutorial2.png");
	tutorial_tex[2].Load("Resource/TutorialTex/UI_Tutorial3.png");
	tutorial_tex[3].Load("Resource/TutorialTex/UI_Tutorial4.png");
	tutorial_tex[4].Load("Resource/TutorialTex/UI_Tutorial5.png");
	tutorial_tex[5].Load("Resource/TutorialTex/UI_Tutorial6.png");
	tutorial_tex[6].Load("Resource/TutorialTex/UI_Tutorial7.png");
	tutorial_tex[7].Load("Resource/TutorialTex/UI_Tutorial8.png");
	tutorial_tex[8].Load("Resource/TutorialTex/UI_Tutorial9.png");
	tutorial_tex[9].Load("Resource/TutorialTex/UI_Tutorial10.png");

	is_tutorial = true;
	tutorial_num = 0;
	tex_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	tex_transform.scale = { 1280.0f, 720.0f };

	input_return = false;
}

void Tutorial::Update()
{
	input_old_return = input_return;
	input_return = KeyBoradInputManager::Instance()->InputState(DIK_RETURN);
	if (!input_old_return && input_return)
	{
		if (!is_next)
		{
			is_next = true;
		}
	}

	if (is_next)
	{
		tutorial_num++;
		is_next = false;
	}
}

void Tutorial::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (!TitleFlag::Instance()->m_isTitle)
	{
		tutorial_tex[tutorial_num].m_transform = tex_transform;
		tutorial_tex[tutorial_num].Draw(arg_rasterize);
	}
}
