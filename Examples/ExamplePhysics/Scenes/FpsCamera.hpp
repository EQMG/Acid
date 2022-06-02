#pragma once

#include <Scenes/Camera.hpp>

using namespace acid;

namespace test {
class FpsCamera : public Camera {
public:
	FpsCamera();

	void Start() override;
	void Update() override;
};
}
