#pragma once
#include "../KazLibrary/Helper/ISinglton.h"

class TitleFlag : public ISingleton<TitleFlag> {

public:

	bool m_isTitle;
	bool m_isQuit;
	bool m_isStart;

public:

	void Init() {
		m_isTitle = true;
		m_isQuit = false;
		m_isStart = false;
	};

};