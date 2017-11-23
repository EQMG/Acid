#include "ComponentGlow.hpp"

#include "../../engine/Engine.hpp"
#include "../Entity.hpp"

namespace Flounder
{
	ComponentGlow::ComponentGlow(Texture *swayMap) :
		IComponent(),
		m_textureGlowMap(swayMap)
	{
	}

	ComponentGlow::~ComponentGlow()
	{
	}

	void ComponentGlow::Update()
	{
	}

	void ComponentGlow::CmdRender(EntityRender *entityRender)
	{
	// 	entityRender->descriptorWrites.push_back(m_textureGlowMap->GetWriteDescriptor(4, entityRender->descriptorSet));
		// TODO
	}
}
