#pragma once

#include <Renderer/RendererManager.hpp>

using namespace acid;

namespace test
{
class MainRenderer :
	public RendererManager
{
public:
	MainRenderer();

	void Update() override;
};
}
