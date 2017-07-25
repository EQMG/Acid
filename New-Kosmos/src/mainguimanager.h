#pragma once

#include <flounder.h>
#include "guis/iguimanager.h"
#include "guis/overlaystartup.h"

using namespace flounder;

class mainguimanager :
	public iguimanager
{
private:
	colour *m_primaryColour;
	overlaystartup *m_overlayStartup;
public:
	mainguimanager();

	~mainguimanager();

	void update() override;

	bool isGamePaused() override;

	float getBlurFactor() override;

	inline colour *getPrimaryColour() override { return m_primaryColour; }
};
