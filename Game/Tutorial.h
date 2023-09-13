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
	std::array<DrawCallUISet, 10> tutorial_tex;
	KazMath::Transform2D tex_transform;

	void setting();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


};
