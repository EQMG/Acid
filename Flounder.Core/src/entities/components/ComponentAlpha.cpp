#include "ComponentAlpha.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentAlpha::ComponentAlpha(const float &alpha) :
		IComponent(),
		m_alpha(alpha)
	{
	}

	ComponentAlpha::~ComponentAlpha()
	{
	}

	void ComponentAlpha::Update()
	{
	}

	void ComponentAlpha::CmdRender(EntityRender *entityRender)
	{
	//	entityRender->uboObject.alpha = m_alpha;
	}
}
