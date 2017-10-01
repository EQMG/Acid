#pragma once

#include "../maths/Matrix4.hpp"
#include "../maths/Vector3.hpp"
#include "../models/Model.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	class Skybox
	{
	private:
		Texture *m_texture;
		Model *m_model;

		float m_size;
		float m_blend;

		Vector3 *m_rotation;
		Matrix4 *m_modelMatrix;
	public:
		Skybox(Texture *texture, Model *model, const float &size);

		~Skybox();

		void Update();

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		Model *GetModel() const { return m_model; }

		void SetModel(Model *model) { m_model = model; }

		float GetSize() const { return m_size; }

		void SetSize(const float size) { m_size = size; }

		float GetBlend() const { return m_blend; }

		void SetBlend(const float blend) { m_blend = blend; }

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(Vector3 *rotation) { m_rotation = rotation; }

		Matrix4 *GetModelMatrix() const { return m_modelMatrix; }
	};
}
