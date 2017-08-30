#pragma once

#include "../camera/camera.hpp"
#include "../maths/vector3.hpp"
#include "../maths/matrix4x4.hpp"
#include "../textures/texture.hpp"
#include "../physics/aabb.hpp"
#include "../models/model.hpp"

namespace flounder
{
	class water
	{
	private:
		model *m_model;

		colour *m_colour;

		vector3 *m_position;
		vector3 *m_rotation;
		vector3 *m_offset;
		bool m_moved;

		matrix4x4 *m_modelMatrix;
		aabb *m_aabb;
	public:
		static const float WAVE_SPEED;
		static const float WAVE_LENGTH;
		static const float AMPLITUDE;

		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;

		static const colour WATER_COLOUR;

		static const float SHINE_DAMPER;
		static const float REFLECTIVITY;

		water(const vector3 &position = vector3(), const vector3 &rotation = vector3());

		~water();

		void update();
	private:
		void generateMesh();

		void storeQuad1(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeQuad2(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeVertex(std::vector<float> *vertices, const int &index, const vector2 &otherPoint1, const vector2 &otherPoint2);

		float encode(const float &x, const float &z, const float &x2, const float &z2);
	public:
		float getHeight(const float &x, const float &z);

		model *getModel() const { return m_model; }

		colour *getColour() const { return m_colour; }

		void setColour(const colour &colour) const { m_colour->set(colour); }

		vector3 *getPosition() const { return m_position; }

		void setPosition(const vector3 &position);

		vector3 *getRotation() const { return m_rotation; }

		void setRotation(const vector3 &rotation);

		vector3 *getOffset() const { return m_offset; }

		matrix4x4 *getModelMatrix() const { return m_modelMatrix; }

		aabb *getAabb() const { return m_aabb; }
	};
}
