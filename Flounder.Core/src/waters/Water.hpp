#pragma once

#include "../camera/Camera.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"
#include "../textures/Texture.hpp"
#include "../physics/Aabb.hpp"
#include "../models/Model.hpp"

namespace Flounder
{
	class water
	{
	private:
		model *m_model;

		Colour *m_colour;

		Vector3 *m_position;
		Vector3 *m_rotation;
		Vector3 *m_offset;
		bool m_moved;

		Matrix4 *m_modelMatrix;
		aabb *m_aabb;
	public:
		static const float WAVE_SPEED;
		static const float WAVE_LENGTH;
		static const float AMPLITUDE;

		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;

		static const Colour WATER_COLOUR;

		static const float SHINE_DAMPER;
		static const float REFLECTIVITY;

		water(const Vector3 &position = Vector3(), const Vector3 &rotation = Vector3());

		~water();

		void update();
	private:
		void generateMesh();

		void storeQuad1(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeQuad2(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeVertex(std::vector<float> *vertices, const int &index, const Vector2 &otherPoint1, const Vector2 &otherPoint2);

		float encode(const float &x, const float &z, const float &x2, const float &z2);
	public:
		float getHeight(const float &x, const float &z);

		model *getModel() const { return m_model; }

		Colour *getColour() const { return m_colour; }

		void setColour(const Colour &colour) const { m_colour->Set(colour); }

		Vector3 *getPosition() const { return m_position; }

		void setPosition(const Vector3 &position);

		Vector3 *getRotation() const { return m_rotation; }

		void setRotation(const Vector3 &rotation);

		Vector3 *getOffset() const { return m_offset; }

		Matrix4 *getModelMatrix() const { return m_modelMatrix; }

		aabb *getAabb() const { return m_aabb; }
	};
}
