#pragma once

#include <Flounder.hpp>

#include "uis/OverlayDebug.hpp"
#include "uis/OverlayManager.hpp"

using namespace Flounder;

class ManagerUis :
	public IManagerUis
{
public:
	static const float SLIDE_TIME;
private:
	Colour *m_primaryColour;

	OverlayStartup *m_overlayStartup;
	OverlayDebug *m_overlayDebug;
	OverlayManager *m_overlayManager;
public:
	ManagerUis();

	~ManagerUis();

	void Update() override;

	bool IsGamePaused() override;

	float GetBlurFactor() override;

	inline Colour *GetPrimaryColour() override { return m_primaryColour; }
};
