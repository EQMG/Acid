#pragma once

#include <Scenes/Camera.hpp>

using namespace acid;

namespace test {
class CameraFree : public Camera {
public:
	CameraFree();

	void Start() override;
	void Update() override;
};
}
