#pragma once

#include "../camera/camera.h"
#include "../maths/vector3.h"
#include "../maths/matrix4x4.h"
#include "../loaders/loaders.h"
#include "../textures/texture.h"
#include "../physics/aabb.h"

namespace flounder
{
	class water
	{
	private:
		GLuint m_vaoID;
		GLuint m_vaoLength;

		colour *m_colour;

		vector3 *m_position;
		vector3 *m_rotation;
		bool m_moved;

		matrix4x4 *m_modelMatrix;
		aabb *m_aabb;
	public:
		static const float WAVE_SPEED;
		static const float WAVE_LENGTH;
		static const float AMPLITUDE;

		static const double SQUARE_SIZE;
		static const int VERTEX_COUNT;

		static const colour WATER_COLOUR;

		static const float SHINE_DAMPER;
		static const float REFLECTIVITY;

		water(const vector3 &position, const vector3 &rotation);

		~water();

		void update();

		/// <summary>
		/// Generates the water mesh and loads it to a VAO.
		/// Generates the {@code float[]} of vertex data which will be loaded to the water's VAO.
		/// The array contains the vertex x and z positions as well as an encoded indication of which corner of its triangle each vertex lies in.
		/// </summary>
	private:
		void generateMesh();

		void storeQuad1(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeQuad2(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed);

		void storeVertex(std::vector<float> *vertices, const int &index, const vector2 &otherPoint1, const vector2 &otherPoint2);

		/// <summary>
		/// Encodes the position of 2 vertices in a triangle (relative to the other vertex) into a single float.
		/// </summary>
		/// <param name="x"> Relative x position of first other vertex. </param>
		/// <param name="z"> Relative z position of first other vertex. </param>
		/// <param name="x2"> Relative x position of second other vertex. </param>
		/// <param name="z2"> Relative z position of second other vertex.
		/// </param>
		/// <returns> The encoded float. </returns>
		float encode(const float &x, const float &z, const float &x2, const float &z2);
	public:
		float getHeight(const float &x, const float &z);

		GLuint getVaoID() const { return m_vaoID; }

		GLuint getVaoLength() const { return m_vaoLength; }

		colour *getColour() const { return m_colour; }

		void setColour(const colour &colour) const { m_colour->set(colour); }

		vector3 *getPosition() const
		{
			return m_position;
		}

		void setPosition(const vector3 &position);

		vector3 *getRotation() const { return m_rotation; }

		void setRotation(const vector3 &rotation);

		matrix4x4 *getModelMatrix() const { return m_modelMatrix; }

		aabb * getAabb() const { return m_aabb; }
	};
}
