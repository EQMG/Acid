#include "BlockFace.hpp"

namespace Flounder
{
	static std::vector<Vertex> VERTICES_FRONT = {
		Vertex(Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.66f), Vector3(0.0f, 0.0f, -1.0f)),
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.25f, 0.66f), Vector3(0.0f, 0.0f, -1.0f)),
		Vertex(Vector3(1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.33f), Vector3(0.0f, 0.0f, -1.0f)),
		Vertex(Vector3(1.0f, -1.0f, -1.0f), Vector2(0.25f, 0.33f), Vector3(0.0f, 0.0f, -1.0f))
	};
	static std::vector<uint32_t> INDICES_FRONT = {
		0, 2, 1,
		1, 2, 3
	};

	static std::vector<Vertex> VERTICES_BACK = {
		Vertex(Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.5f, 0.66f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(1.0f, -1.0f, 1.0f), Vector2(0.5f, 0.33f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 1.0f), Vector2(0.75f, 0.66f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(1.0f, 1.0f, 1.0f), Vector2(0.75f, 0.33f), Vector3(0.0f, 0.0f, 1.0f))
	};
	static std::vector<uint32_t> INDICES_BACK = {
		0, 1, 2,
		1, 3, 2
	};

	static std::vector<Vertex> VERTICES_TOP = {
		Vertex(Vector3(-1.0f, 1.0f, 1.0f), Vector2(0.75f, 0.66f), Vector3(0.0f, 1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 1.0f), Vector2(0.75f, 0.33f), Vector3(0.0f, 1.0f, 0.0f)),
		Vertex(Vector3(-1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.66f), Vector3(0.0f, 1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.33f), Vector3(0.0f, 1.0f, 0.0f))
	};
	static std::vector<uint32_t> INDICES_TOP = {
		0, 1, 2,
		1, 3, 2
	};

	static std::vector<Vertex> VERTICES_BOTTOM = {
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.25f, 0.66f), Vector3(0.0f, -1.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, -1.0f), Vector2(0.25f, 0.33f), Vector3(0.0f, -1.0f, 0.0f)),
		Vertex(Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.5f, 0.66f), Vector3(0.0f, -1.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, 1.0f), Vector2(0.5f, 0.33f), Vector3(0.0f, -1.0f, 0.0f))
	};
	static std::vector<uint32_t> INDICES_BOTTOM = {
		0, 1, 2,
		1, 3, 2
	};

	static std::vector<Vertex> VERTICES_LEFT = {
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.25f, 0.66f), Vector3(0.0f, 0.0f, -1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 1.0f), Vector2(0.5f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.25f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		Vertex(Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.5f, 0.66f), Vector3(0.0f, 0.0f, -1.0f))
	};
	static std::vector<uint32_t> INDICES_LEFT = {
		0, 1, 2,
		0, 3, 1
	};

	static std::vector<Vertex> VERTICES_RIGHT = {
		Vertex(Vector3(1.0f, -1.0f, -1.0f), Vector2(0.25f, 0.33f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(1.0f, 1.0f, -1.0f), Vector2(0.25f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(1.0f, -1.0f, 1.0f), Vector2(0.5f, 0.33f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f))
	};
	static std::vector<uint32_t> INDICES_RIGHT = {
		0, 1, 2,
		2, 1, 3
	};

	BlockFace::BlockFace(const FaceType &faceType, const short &blockType) :
		m_faceType(faceType),
		m_blockType(blockType)
	{
	}

	void BlockFace::AppendVertices(std::vector<Vertex> *vertices, const Vector3 &position, const Vector3 &scale) const
	{
		std::vector<Vertex> data = {};

		switch (m_faceType)
		{
		case TypeFront:
			data = VERTICES_FRONT;
			break;
		case TypeBack:
			data = VERTICES_BACK;
			break;
		case TypeTop:
			data = VERTICES_TOP;
			break;
		case TypeBottom:
			data = VERTICES_BOTTOM;
			break;
		case TypeLeft:
			data = VERTICES_LEFT;
			break;
		case TypeRight:
			data = VERTICES_RIGHT;
			break;
		default:
			break;
		}

		for (const auto &vertex : data)
		{
			Vertex v = Vertex(vertex);
			v.m_position = (scale * v.m_position) + position;
			v.m_tangent = Colour("#5E7831");
			vertices->push_back(v);
		}
	}

	void BlockFace::AppendIndices(std::vector<uint32_t> *indices, unsigned int indexStart) const
	{
		std::vector<uint32_t> data = {};

		switch (m_faceType)
		{
		case TypeFront:
			data = INDICES_FRONT;
			break;
		case TypeBack:
			data = INDICES_BACK;
			break;
		case TypeTop:
			data = INDICES_TOP;
			break;
		case TypeBottom:
			data = INDICES_BOTTOM;
			break;
		case TypeLeft:
			data = INDICES_LEFT;
			break;
		case TypeRight:
			data = INDICES_RIGHT;
			break;
		default:
			break;
		}

		for (const auto &index : data)
		{
			indices->push_back(index + indexStart);
		}
	}

	bool BlockFace::operator==(const BlockFace &other) const
	{
		return m_faceType == other.m_faceType; // && m_position == other.m_position && m_scale == other.m_scale;
	}

	bool BlockFace::operator!=(const BlockFace &other) const
	{
		return !(*this == other);
	}
}
