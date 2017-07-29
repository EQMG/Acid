#pragma once

#include <flounder.h>
#include "uis/imanageruis.h"
#include "uis/overlaystartup.h"

#include "uis/overlaydebug.h"
#include "uis/overlaymanager.h"

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
