#pragma once

#include <flounder.hpp>
#include "uis/imanageruis.hpp"
#include "uis/overlaystartup.hpp"

#include "uis/overlaydebug.hpp"
#include "uis/overlaymanager.hpp"

using namespace flounder;

class manageruis :
	public imanageruis
{
public:
	static const float SLIDE_TIME;
private:
	colour *m_primaryColour;

	overlaystartup *m_overlayStartup;
	overlaydebug *m_overlayDebug;
	overlaymanager *m_overlayManager;
public:
	manageruis();

	~manageruis();

	void update() override;

	bool isGamePaused() override;

	float getBlurFactor() override;

	inline colour *getPrimaryColour() override { return m_primaryColour; }
};
