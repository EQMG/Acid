#pragma once

#include "../camera/Camera.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"
#include "../textures/Texture.hpp"
#include "../physics/Aabb.hpp"
#include "../models/Model.hpp"

namespace Flounder
{
	class Water
	{
	private:
		Model *m_model;

		Colour *m_colour;

		Vector3 *m_position;
		Vector3 *m_rotation;
		Vector3 *m_offset;
		bool m_moved;

		Matrix4 *m_modelMatrix;
		Aabb *m_aabb;
	public:
		static const float WAVE_SPEED;
		static const float WAVE_LENGTH;
		static const float AMPLITUDE;

		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;

		static const Colour WATER_COLOUR;

		static const float SHINE_DAMPER;
		static const float REFLECTIVITY;

		Water(const Vector3 &position = Vector3(), const Vector3 &rotation = Vector3());

		~Water();

		void Update();
	private:
		void GenerateMesh();

		void StoreQuad1(std::vector<float> &vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed) const;

		void StoreQuad2(std::vector<float> &vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed) const;

		void StoreVertex(std::vector<float> &vertices, const int &index, const Vector2 &otherPoint1, const Vector2 &otherPoint2) const;

		float Encode(const float &x, const float &z, const float &x2, const float &z2) const;
	public:
		float GetHeight(const float &x, const float &z) const;

		Model *GetModel() const { return m_model; }

		Colour *GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) const { m_colour->Set(colour); }

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position);

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation);

		Vector3 *GetOffset() const { return m_offset; }

		Matrix4 *GetModelMatrix() const { return m_modelMatrix; }

		Aabb *GetAabb() const { return m_aabb; }
	};
}
