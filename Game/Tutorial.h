#pragma once
#include "../Game/DrawCallSet.h"
#include "../Game/DrawCallUISet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class Tutorial : public ISingleton<Tutorial> {

private:


public:

	bool input_return;
	bool input_old_return;
	bool is_next;
	bool is_tutorial;
	int tutorial_num;
	float ease_time;
	const float EASE_TIME_MAX = 30.0f;
	std::array<DrawCallUISet, 12> tutorial_tex;
	KazMath::Transform2D tex_transform;
	//�`���[�g���A�����΂������Ɏg���ϐ�����
	float move_amount;
	float tutorial_timer;
	float TUTORIAL_TIME_MAX;
	bool spawn_minekuzi;
	bool spawn_m_rock;
	bool spawn_rock;
	bool break_mineral;
	bool spawn_tree;
	int tree_carry_count;
	float draw_timer;
	float DRAW_TIME_MAX = 120.0f;

	void setting();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


};
