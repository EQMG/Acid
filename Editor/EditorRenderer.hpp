#pragma once

#include <Graphics/Renderer.hpp>

using namespace acid;

namespace test {
class EditorRenderer : public Renderer {
public:
	EditorRenderer();

	void Start() override;
	void Update() override;
};
}
