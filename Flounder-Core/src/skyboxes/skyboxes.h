#pragma once

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../lights/fog.h"
#include "../maths/matrix4x4.h"
#include "../maths/vector3.h"
#include "../models/model.h"
#include "../textures/texture.h"

namespace flounder
{
	class skyboxes :
		public imodule
	{
	private:
		float m_size;
		texture *m_texture;
		model *m_model;

		vector3 *m_rotation;
		matrix4x4 *m_modelMatrix;
		fog *m_fog;
		float m_blend;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static skyboxes *get()
		{
			return static_cast<skyboxes*>(framework::get()->getInstance("skyboxes"));
		}

		skyboxes();

		~skyboxes();

		void load(texture *texture, model *model);

		void update() override;

		inline float &getSize() { return m_size; }

		inline void setSize(const float size) { m_size = size; }

		inline texture *getTexture() { return m_texture; }

		inline void setTexture(texture *texture) { m_texture = texture; }

		inline model *getModel() { return m_model; }

		inline void setModel(model *model) { m_model = model; }

		inline vector3 *getRotation() { return m_rotation; }

		inline void setRotation(vector3 *rotation) { m_rotation = rotation; }

		inline matrix4x4 *getModelMatrix() { return m_modelMatrix; }

		inline fog *getFog() { return m_fog; }

		inline void setFog(fog *fog) { m_fog = fog; }

		inline float &getBlend() { return m_blend; }

		inline void setBlend(const float blend) { m_blend = blend; }
	};
}
