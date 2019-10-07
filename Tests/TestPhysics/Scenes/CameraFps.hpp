#pragma once

#include <Scenes/Camera.hpp>

using namespace acid;

namespace test {
class CameraFps : public Camera {
public:
	CameraFps();

	void Start() override;
	void Update() override;
};
}
