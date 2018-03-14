#include "Chunk.hpp"

#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	const unsigned int Chunk::SIDE_LENGTH = 32;
	const unsigned int Chunk::MAX_HEIGHT = 16;

	Chunk::Chunk(const bool &generate) :
		Component(),
		m_blocks(new BlockVector()),
		m_generate(generate),
		m_rebuild(true)
	{
		for (unsigned int x = 0; x < SIDE_LENGTH; x++)
		{
			m_blocks->push_back(std::vector<std::vector<Block*>>());

			for (unsigned int z = 0; z < SIDE_LENGTH; z++)
			{
				m_blocks->at(x).push_back(std::vector<Block*>());

				for (unsigned int y = 0; y < MAX_HEIGHT; y++)
				{
					m_blocks->at(x).at(z).push_back(nullptr);
				}
			}
		}
	}

	Chunk::~Chunk()
	{
		for (auto x : *m_blocks)
		{
			for (auto z : x)
			{
				for (auto y : z)
				{
					delete y;
				}
			}
		}

		delete m_blocks;
	}

	void Chunk::Update()
	{
		if (m_generate)
		{
			Generate();
			m_generate = false;
		}

		if (m_rebuild)
		{
			GenerateMesh();
			m_rebuild = false;
		}
	}

	void Chunk::Load(LoadedValue *value)
	{
	}

	void Chunk::Write(LoadedValue *value)
	{
	}

	Block *Chunk::GetBlock(const unsigned int &x, const unsigned int &y, const unsigned int &z)
	{
		if (x < 0 || x >= SIDE_LENGTH || z < 0 || z >= SIDE_LENGTH || y < 0 || y >= MAX_HEIGHT)
		{
			return nullptr;
		}

		return m_blocks->at(x).at(z).at(y);
	}

	bool Chunk::IsBlockFilled(const unsigned int &x, const unsigned int &y, const unsigned int &z)
	{
		Block *block = GetBlock(x, y, z);

		return block == nullptr || !block->IsFilled();
	}

	void Chunk::Generate()
	{
		auto position = *GetGameObject()->GetTransform()->GetPosition() / 2.0f;
		auto noise = Worlds::Get()->GetNoise();

		for (unsigned int x = 0; x < SIDE_LENGTH; x++)
		{
			for (unsigned int z = 0; z < SIDE_LENGTH; z++)
			{
				for (unsigned int y = 0; y < MAX_HEIGHT; y++)
				{
					if (noise->GetValue(x + position.m_x, y + position.m_y, z + position.m_z) < 0.1f)
					{
						continue;
					}

					Block *block = new Block(this, 2.0f * Vector3(x, y, z), Colour("#629632"));
					m_blocks->at(x).at(z).at(y) = block;
				}
			}
		}
	}

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

	void Chunk::GenerateMesh()
	{
#if FLOUNDER_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr)
		{
			return;
		}

		delete mesh->GetModel();

		std::vector<Vertex> vertices = {};
		std::vector<uint32_t> indices = {};

		for (unsigned int x = 0; x < SIDE_LENGTH; x++)
		{
			for (unsigned int z = 0; z < SIDE_LENGTH; z++)
			{
				for (unsigned int y = 0; y < MAX_HEIGHT; y++)
				{
					Block *block = GetBlock(x, y, z);

					if (block == nullptr || !block->IsFilled())
					{
						continue;
					}

					if (IsBlockFilled(x, y, z - 1))
					{
						AppendFace(VERTICES_FRONT, INDICES_FRONT, *block, &vertices, &indices);
					}
					if (IsBlockFilled(x, y, z + 1))
					{
						AppendFace(VERTICES_BACK, INDICES_BACK, *block, &vertices, &indices);
					}
					if (IsBlockFilled(x, y + 1, z))
					{
						AppendFace(VERTICES_TOP, INDICES_TOP, *block, &vertices, &indices);
					}
					if (IsBlockFilled(x, y - 1, z))
					{
						AppendFace(VERTICES_BOTTOM, INDICES_BOTTOM, *block, &vertices, &indices);
					}
					if (IsBlockFilled(x - 1, y, z))
					{
						AppendFace(VERTICES_LEFT, INDICES_LEFT, *block, &vertices, &indices);
					}
					if (IsBlockFilled(x + 1, y, z))
					{
						AppendFace(VERTICES_RIGHT, INDICES_RIGHT, *block, &vertices, &indices);
					}
				}
			}
		}

		if (!vertices.empty() && !indices.empty())
		{
			Model *model = new Model(vertices, indices, GetName());
			mesh->SetModel(model);
		}

#if FLOUNDER_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();

		if (debugEnd - debugStart > 22.0f)
		{
			printf("Chunk %s built in %fms\n", GetName().c_str(), debugEnd - debugStart);
		}
#endif
	}

	void Chunk::AppendFace(const std::vector<Vertex> &dataVertices, const std::vector<uint32_t> &dataIndices, const Block &block, std::vector<Vertex> *vertices, std::vector<uint32_t> *indices)
	{
		unsigned int indexStart = vertices->size();

		for (const auto &v : dataVertices)
		{
			Vertex vertex = Vertex(v);
			vertex.m_position = vertex.m_position + *block.GetPosition();
			vertex.m_tangent = *block.GetColour();
			vertices->push_back(vertex);
		}

		for (const auto &i : dataIndices)
		{
			uint32_t index = i + indexStart;
			indices->push_back(index);
		}
	}
}
