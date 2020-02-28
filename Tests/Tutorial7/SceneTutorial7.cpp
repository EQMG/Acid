#include "SceneTutorial7.hpp"
#include "Devices/Window.hpp"
#include "Meshes/Mesh.hpp"
#include "Models/Obj/ModelObj.hpp"
#include "SimpleMaterial.hpp"
#include "Resources/Resources.hpp"

/*
 * OK, now we are using Acid Scenes
 */
void SceneTutorial7::Start() {
	/*
	 * Get the camera and move it an point it at the origin
	 */
	auto camera = dynamic_cast<FixedCamera *>(GetCamera());
	if (camera) {
		camera->SetPosition(Vector3f(2.0f, 2.0f, 2.0f));
		camera->LookAt(Vector3f(0, 0, 0), Vector3f(0, 0, 1));
	}

	/*
	 * Create an entity
	 */
	auto mesh = GetStructure()->CreateEntity();
	/*
	 * Give it a default transform
	 */
	mesh->AddComponent<Transform>();

	/*
	 * Now we will add a mesh to this entity using the obj file and a simple unlit material
	 * that has a diffuse texture
	 */
	mesh->AddComponent<Mesh>(ModelObj::Create("chalet/chalet.obj"),
		std::make_unique<SimpleMaterial>(Pipeline::Stage{0, 0}, std::make_shared<Image2d>("chalet/chalet.jpg"))
		);
}

void SceneTutorial7::Update() {
}

/*
 * Update the camera matrices
 */
void FixedCamera::LookAt(Vector3f pos, Vector3f up) {
	m_viewMatrix = Matrix4::LookAt(m_position, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
	m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());
	m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
	m_viewRay.Update(m_position, {0.5f, 0.5f}, m_viewMatrix, m_projectionMatrix);
}
