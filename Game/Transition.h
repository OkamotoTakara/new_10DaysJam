#pragma once
#include "DrawCallUISet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class Transition : public ISingleton<Transition> {

private:

	DrawCallUISet m_transitionUI;
	bool m_isActive;
	enum class STATUS {
		APPEAR,
		STAY,
		EXIT
	}m_status;
	float m_easingTimer;
	const float APPEAR_TIME = 30.0f;
	const float STAY_TIME = 20.0f;
	const float EXIT_TIME = 30.0f;

public:

	void Setting();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void Activate();

	bool GetIsActive() { return m_isActive; }

};