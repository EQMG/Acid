#pragma once

#include <flounder.h>
#include "uis/imanageruis.h"
#include "uis/overlaystartup.h"

#include "uis/overlaydebug.h"

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
public:
	manageruis();

	~manageruis();

	void update() override;

	bool isGamePaused() override;

	float getBlurFactor() override;

	inline colour *getPrimaryColour() override { return m_primaryColour; }
};
