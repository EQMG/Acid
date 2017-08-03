#pragma once

#include <vector>
#include <GL/glew.h>

#include "../loaders/loaders.h"
#include "../maths/vector3.h"
#include "../maths/matrix4x4.h"
#include "../noise/noisesimplex.h"
#include "../physics/aabb.h"

namespace flounder
{
	class terrain
	{
	private:
		GLuint m_vaoID;
		GLuint m_vaoLength;

		vector3 *m_position;
		vector3 *m_rotation;
		bool m_moved;

		matrix4x4 *m_modelMatrix;
		aabb *m_aabb;
	public:
		static const double SQUARE_SIZE;
		static const int VERTEX_COUNT;

		terrain(const vector3 &position, const vector3 &rotation);

		~terrain();

		void update();
	private:
		void generateMesh();

		vector3 calculateNormal(const int &x, const int &z, noisesimplex &noise);

		vector3 calculatePosition(const int &x, const int &z, noisesimplex &noise);
	public:
		GLuint getVaoID() const { return m_vaoID; }

		GLuint getVaoLength() const { return m_vaoLength; }

		vector3 *getPosition() const { return m_position; }

		void setPosition(const vector3 &position);

		vector3 *getRotation() const { return m_rotation; }

		void setRotation(const vector3 &rotation);

		matrix4x4 *getModelMatrix() const { return m_modelMatrix; }

		aabb *getAabb() const { return m_aabb; }
	};
}
