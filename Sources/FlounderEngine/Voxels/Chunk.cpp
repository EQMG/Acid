#include "Chunk.hpp"

#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	const unsigned int Chunk::SIDE_LENGTH = 32;

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

				for (unsigned int y = 0; y < SIDE_LENGTH; y++)
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
		if (x < 0 || x >= SIDE_LENGTH || z < 0 || z >= SIDE_LENGTH || y < 0 || y >= SIDE_LENGTH)
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
				for (unsigned int y = 0; y < SIDE_LENGTH; y++)
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

		std::vector<BlockFace> faces = {};

		for (unsigned int x = 0; x < SIDE_LENGTH; x++)
		{
			for (unsigned int z = 0; z < SIDE_LENGTH; z++)
			{
				for (unsigned int y = 0; y < SIDE_LENGTH; y++)
				{
					Block *block = GetBlock(x, y, z);

					if (block == nullptr || !block->IsFilled())
					{
						continue;
					}

					SearchFaces(x, y, z, block->GetColour(), &faces);
				}
			}
		}

		/*for (const auto &face1 : faces)
		{
			for (const auto &face2 : faces)
			{
				if (face1 == face2)
				{
					continue;
				}

				if (BlockFace::CompareFaces(face1, face2))
				{
				//	printf("Can combine\n");
				}
			}
		}*/

		std::vector<Vertex> vertices = {};
		std::vector<uint32_t> indices = {};

		for (const auto &face : faces)
		{
			unsigned int indexStart = vertices.size();

			face.AppendVertices(&vertices);
			face.AppendIndices(&indices, indexStart);
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

	void Chunk::SearchFaces(const unsigned int &x, const unsigned int &y, const unsigned int &z, Colour *colour, std::vector<BlockFace> *faces)
	{
		Vector3 position = 2.0f * Vector3(x, y, z);

		if (IsBlockFilled(x, y, z - 1))
		{
			faces->push_back(BlockFace(BlockFaceType::TypeFront, position, colour));
		}

		if (IsBlockFilled(x, y, z + 1))
		{
			faces->push_back(BlockFace(BlockFaceType::TypeBack, position, colour));
		}

		if (IsBlockFilled(x, y + 1, z))
		{
			faces->push_back(BlockFace(BlockFaceType::TypeTop, position, colour));
		}

		if (IsBlockFilled(x, y - 1, z))
		{
			faces->push_back(BlockFace(BlockFaceType::TypeBottom, position, colour));
		}

		if (IsBlockFilled(x - 1, y, z))
		{
			faces->push_back(BlockFace(BlockFaceType::TypeLeft, position, colour));
		}

		if (IsBlockFilled(x + 1, y, z))
		{
			faces->push_back(BlockFace(BlockFaceType::TypeRight, position, colour));
		}
	}
}
