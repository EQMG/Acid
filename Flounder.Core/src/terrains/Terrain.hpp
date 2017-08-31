#pragma once

#include <vector>

#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"
#include "../physics/aabb.hpp"
#include "../models/model.hpp"
#include "../worlds/Worlds.hpp"

#include "biome.hpp"

namespace Flounder
{
	class terrain
	{
	private:
		model *m_model;

		Vector3 *m_position;
		Vector3 *m_rotation;
		bool m_moved;

		Matrix4 *m_modelMatrix;
		aabb *m_aabb;
	public:
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;
		static const float SIDE_LENGTH;

		terrain(const Vector3 &position, const Vector3 &rotation, const int &seed);

		~terrain();

		void update();
	private:
		void generateMesh();

		void storeQuad1(std::vector<int> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeQuad2(std::vector<int> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		Vector3 calculateNormal(const float &x, const float &z);

		Colour getBiomeColour(const float &x, const float &z);

		float getFactorIsland(const float &x, const float &z);

		float getFactorMoisture(const float &x, const float &y, const float &z);
	public:
		float getHeight(const float &x, const float &z);

		model *getModel() const { return m_model; }

		Vector3 *getPosition() const { return m_position; }

		void setPosition(const Vector3 &position);

		Vector3 *getRotation() const { return m_rotation; }

		void setRotation(const Vector3 &rotation);

		Matrix4 *getModelMatrix() const { return m_modelMatrix; }

		aabb *getAabb() const { return m_aabb; }
	};
}
