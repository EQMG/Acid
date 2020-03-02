#include "SceneTutorial7.hpp"
#include "Devices/Window.hpp"
#include "Meshes/Mesh.hpp"
#include "Models/Obj/ObjModel.hpp"
#include "SimpleMaterial.hpp"
#include "Resources/Resources.hpp"

/*
 * OK, now we are using Acid Scenes
 */
void SceneTutorial7::Start() {
	// Get the camera and move it an point it at the origin.
	auto camera = dynamic_cast<FixedCamera *>(GetCamera());
	if (camera) {
		camera->SetPosition(Vector3f(2.0f, 2.0f, 2.0f));
		camera->LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
	}

	// Create an entity.
	auto mesh = GetStructure()->CreateEntity();
	// Give it a default transform.
	mesh->AddComponent<Transform>();

	// Now we will add a mesh to this entity using the obj file and a simple unlit material
	// that has a diffuse texture
	mesh->AddComponent<Mesh>(ObjModel::Create("chalet/chalet.obj"),
		std::make_unique<SimpleMaterial>(Pipeline::Stage{0, 0}, std::make_shared<Image2d>("chalet/chalet.jpg"))
		);
}

void SceneTutorial7::Update() {
}

/*
 * Update the camera matrices
 */
void FixedCamera::LookAt(Vector3f pos, Vector3f up) {
	viewMatrix = Matrix4::LookAt(this->position, pos, up);
	projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());
	viewFrustum.Update(viewMatrix, projectionMatrix);
	viewRay.Update(this->position, {0.5f, 0.5f}, viewMatrix, projectionMatrix);
}
