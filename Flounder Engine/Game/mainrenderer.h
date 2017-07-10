#pragma once

#include "../Sources/renderer/irenderermaster.h"

#include "../Sources/devices/mouse.h"

namespace flounder {
	class mainrenderer : public irenderermaster
	{
	public:
		mainrenderer();

		~mainrenderer();

		void render() override;
	};
}
