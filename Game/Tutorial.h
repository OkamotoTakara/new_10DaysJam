#pragma once
#include "../Game/DrawCallSet.h"
#include "../Game/DrawCallUISet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class Tutorial : public ISingleton<Tutorial> {

private:




public:

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


};