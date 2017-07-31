#pragma once

#include <flounder.h>

using namespace flounder;

class overlaydebug :
	public uiobject
{
private:
	text *m_textFps;
	text *m_textUps;
	timer *m_timerUpdate;
public:
	overlaydebug(uiobject *parent);

	~overlaydebug();

	virtual void updateObject() override;
private:
	text *createStatus(const std::string &content, const float &positionX, const float &positionY, const uialign &align);
};
