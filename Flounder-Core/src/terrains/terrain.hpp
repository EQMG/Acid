#pragma once

#include <vector>

#include "../maths/vector3.hpp"
#include "../maths/matrix4x4.hpp"
#include "../physics/aabb.hpp"
#include "../models/model.hpp"
#include "../worlds/worlds.hpp"

#include "biome.hpp"

namespace flounder
{
	class terrain
	{
	private:
		model *m_model;

		vector3 *m_position;
		vector3 *m_rotation;
		bool m_moved;

		matrix4x4 *m_modelMatrix;
		aabb *m_aabb;
	public:
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;
		static const float SIDE_LENGTH;

		terrain(const vector3 &position, const vector3 &rotation, const int &seed);

		~terrain();

		void update();
	private:
		void generateMesh();

		void storeQuad1(std::vector<int> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeQuad2(std::vector<int> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		vector3 calculateNormal(const float &x, const float &z);

		colour getBiomeColour(const float &x, const float &z);

		float getFactorIsland(const float &x, const float &z);

		float getFactorMoisture(const float &x, const float &y, const float &z);
	public:
		float getHeight(const float &x, const float &z);

		model *getModel() const { return m_model; }

		vector3 *getPosition() const { return m_position; }

		void setPosition(const vector3 &position);

		vector3 *getRotation() const { return m_rotation; }

		void setRotation(const vector3 &rotation);

		matrix4x4 *getModelMatrix() const { return m_modelMatrix; }

		aabb *getAabb() const { return m_aabb; }
	};
}
