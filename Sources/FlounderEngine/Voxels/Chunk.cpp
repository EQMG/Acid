#include "Chunk.hpp"

#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	const Vector3 *Chunk::S_CHUNK_SIZE = new Vector3(16.0f, 16.0f, 16.0f);

	Chunk::Chunk(const bool &generate) :
		Component(),
		m_position(new Vector3()),
		m_blocks(new BlockVector()),
		m_generate(generate),
		m_rebuild(true)
	{
		for (unsigned int x = 0; x < S_CHUNK_SIZE->m_x; x++)
		{
			m_blocks->push_back(std::vector<std::vector<Block*>>());

			for (unsigned int z = 0; z < S_CHUNK_SIZE->m_z; z++)
			{
				m_blocks->at(x).push_back(std::vector<Block*>());

				for (unsigned int y = 0; y < S_CHUNK_SIZE->m_y; y++)
				{
					m_blocks->at(x).at(z).push_back(nullptr);
				}
			}
		}
	}

	Chunk::~Chunk()
	{
		delete m_position;

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

		// TODO: Wrap chunk positions around planet to follow camera.
	}

	void Chunk::Load(LoadedValue *value)
	{
		// TODO: Not from game object but from saved values.
	}

	void Chunk::Write(LoadedValue *value)
	{
		// TODO: Not to game object but to saves.
	}

	Block *Chunk::GetBlock(const unsigned int &x, const unsigned int &y, const unsigned int &z)
	{
		if (x < 0 || x >= S_CHUNK_SIZE->m_x || z < 0 || z >= S_CHUNK_SIZE->m_z || y < 0 || y >= S_CHUNK_SIZE->m_y)
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

	bool Chunk::IsFaceVisible(const unsigned int &x, const unsigned int &y, const unsigned int &z, const BlockFaceType &faceType)
	{
		switch (faceType)
		{
		case TypeFront:
			return IsBlockFilled(x, y, z - 1);
		case TypeBack:
			return IsBlockFilled(x, y, z + 1);
		case TypeTop:
			return IsBlockFilled(x, y + 1, z);
		case TypeBottom:
			return IsBlockFilled(x, y - 1, z);
		case TypeLeft:
			return IsBlockFilled(x - 1, y, z);
		case TypeRight:
			return IsBlockFilled(x + 1, y, z);
		}

		return false;
	}

	void Chunk::Generate()
	{
		*m_position = *GetGameObject()->GetTransform()->GetPosition();
		auto position = *GetGameObject()->GetTransform()->GetPosition() / 2.0f;
		auto noise = Worlds::Get()->GetNoise();

		for (unsigned int x = 0; x < S_CHUNK_SIZE->m_x; x++)
		{
			for (unsigned int z = 0; z < S_CHUNK_SIZE->m_z; z++)
			{
				for (unsigned int y = 0; y < S_CHUNK_SIZE->m_y; y++)
				{
					if (noise->GetValue(x + position.m_x, y + position.m_y, z + position.m_z) < 0.1f)
					{
						continue;
					}

					Block *block = new Block(this, 2.0f * Vector3(x, y, z), 12);
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

		std::vector<Vertex> vertices = {};
		std::vector<uint32_t> indices = {};

		CreateSimpleMesh(&vertices, &indices);
	//	CreateGreedyMesh(&vertices, &indices);

		delete mesh->GetModel();

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

	void Chunk::CreateSimpleMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices)
	{
		std::vector<BlockFace> faces = {};

		for (unsigned int x = 0; x < S_CHUNK_SIZE->m_x; x++)
		{
			for (unsigned int z = 0; z < S_CHUNK_SIZE->m_z; z++)
			{
				for (unsigned int y = 0; y < S_CHUNK_SIZE->m_y; y++)
				{
					Block *block = GetBlock(x, y, z);

					if (block == nullptr || !block->IsFilled())
					{
						continue;
					}

					Vector3 position = 2.0f * Vector3(x, y, z);

					for (unsigned int faceType = 0; faceType < 6; faceType++)
					{
						if (IsFaceVisible(x, y, z, (BlockFaceType)faceType))
						{
							faces.push_back(BlockFace((BlockFaceType)faceType, position, block->GetType()));
						}
					}
				}
			}
		}

		for (const auto &face : faces)
		{
			unsigned int indexStart = vertices->size();

			face.AppendVertices(vertices);
			face.AppendIndices(indices, indexStart);
		}
	}

	void Chunk::CreateGreedyMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices)
	{
		// https://github.com/Alan-FGR/GreedyMesh
		const int SIZE = 1 << 4;

		std::vector<int> dimensions = {SIZE, SIZE, SIZE};

		// Sweep over 3-axes.
		for (int d = 0; d < 3; ++d)
		{
			int i, j, k, l, w, h, u = (d + 1) % 3, v = (d + 2) % 3;
			std::vector<int> x = std::vector<int>(3);
			std::vector<int> q = std::vector<int>(3);

			std::vector<short> mask = std::vector<short>(dimensions[u] * dimensions[v]);

			q[d] = 1;

			for (x[d] = -1; x[d] < dimensions[d];)
			{
				// Compute mask.
				int n = 0;

				for (x[v] = 0; x[v] < dimensions[v]; ++x[v])
				{
					for (x[u] = 0; x[u] < dimensions[u]; ++x[u])
					{
						mask[n++] = (0 <= x[d] && (GetBlock(x[0], x[1], x[2]) != nullptr)) != (x[d] < dimensions[d] - 1 && (GetBlock(x[0] + q[0], x[1] + q[1], x[2] + q[2]) != nullptr));
					}
				}

				// Increment x[d].
				++x[d];

				// Generate mesh for mask using lexicographic ordering.
				n = 0;

				for (j = 0; j < dimensions[v]; ++j)
				{
					for (i = 0; i < dimensions[u];)
					{
						if (mask[n] != 0)
						{
							// Compute width.
							for (w = 1; mask[n + w] && i + w < dimensions[u]; ++w)
							{
							}

							// Compute height (this is slightly awkward).
							bool done = false;

							for (h = 1; j + h < dimensions[v]; ++h)
							{
								for (k = 0; k < w; ++k)
								{
									if (!mask[n + k + h * dimensions[u]])
									{
										done = true;
										break;
									}
								}
								if (done)
								{
									break;
								}
							}

							// Add quad.
							x[u] = i;
							x[v] = j;

							std::vector<int> du = std::vector<int>(3);
							du[u] = w;
							std::vector<int> dv = std::vector<int>(3);
							dv[v] = h;

							unsigned int indexStart = vertices->size();
							BlockFace face = BlockFace(x, du, dv, mask[n]);
							face.AppendVertices(vertices);
							face.AppendIndices(indices, indexStart);

							// Zero-out mask.
							for (l = 0; l < h; ++l)
							{
								for (k = 0; k < w; ++k)
								{
									mask[n + k + l * dimensions[u]] = false;
								}
							}

							// Increment counters and continue.
							i += w;
							n += w;
						}
						else
						{
							++i;
							++n;
						}
					}
				}
			}
		}
	}
}
