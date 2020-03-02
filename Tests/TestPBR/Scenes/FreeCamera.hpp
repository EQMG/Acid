#pragma once

#include <Scenes/Camera.hpp>

using namespace acid;

namespace test {
class FreeCamera : public Camera {
public:
	FreeCamera();

	void Start() override;
	void Update() override;
};
}
