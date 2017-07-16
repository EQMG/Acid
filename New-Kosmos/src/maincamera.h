#pragma once

#include "devices/display.h"
#include "devices/mouse.h"
#include "camera/icamera.h"

namespace flounder {
	class maincamera : 
		public icamera 
	{
	private:
		vector3 *m_position;
		vector3 *m_rotation;

		matrix4x4 *m_viewMatrix;
		matrix4x4 *m_projectionMatrix;

		frustum *m_viewFrustum;
		ray *m_viewRay;
	public:
		maincamera();

		~maincamera();

		float getNearPlane() override;

		float getFarPlane() override;

		float getFOV() override;

		void update(iplayer *player) override;

		inline frustum *getViewFrustum() override { return m_viewFrustum; }

		inline ray *getViewRay() override { return m_viewRay; }

		inline matrix4x4 *getViewMatrix() override { return m_viewMatrix; }

		inline matrix4x4 *getProjectionMatrix() override { return m_projectionMatrix; }

		void reflect(const float &waterHeight) override;

		inline vector3 *getPosition() override { return m_position; }

		inline vector3 *getRotation() override { return m_rotation; }

		inline void setRotation(vector3 *rotation) override { m_rotation->set(*rotation); }
	};
}
