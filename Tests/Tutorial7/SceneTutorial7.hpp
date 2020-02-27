#pragma once
#include "Scenes/Scene.hpp"
#include "Models/Model.hpp"

using namespace acid;

/*
 * A Camera that cant be controlled but can be pointed at things
 */
class FixedCamera : public Camera
{
public:
	
	void SetPosition(Vector3f position)
	{
		m_position = position;
	}

	void LookAt(Vector3f pos, Vector3f up);
};

/*
 * We are using Acid scenes now
 */
class SceneTutorial7 : public Scene
{
public:
	SceneTutorial7() : Scene(std::make_unique<FixedCamera>()) {};

	void Start() override;
	void Update() override;
	bool IsPaused() const override { return false; };
};
