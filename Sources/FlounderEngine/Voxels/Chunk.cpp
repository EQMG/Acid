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
		// Based off of: https://github.com/Sleaker/Cubed
		int C_SIZE = 16;
		int i, j, k, l, h, w, u, v, n, r, s, t;

		BlockFaceType faceType = BlockFaceType::TypeFront;
		std::vector<int> x = std::vector<int>{0, 0, 0};
		std::vector<int> q = std::vector<int>{0, 0, 0};
		std::vector<int> du = {0, 0, 0};
		std::vector<int> dv = {0, 0, 0};

		std::vector<char> mask = std::vector<char>(C_SIZE * C_SIZE);

		// First pass is for front face, second pass is for backfaces.
		for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
		{
			// Loop over all 3 dimensions.
			for (int d = 0; d < 3; d++)
			{
				u = (d + 1) % 3;
				v = (d + 2) % 3;

				x[0] = 0;
				x[1] = 0;
				x[2] = 0;
				q[0] = 0;
				q[1] = 0;
				q[2] = 0;
				q[d] = 1;

				// Keep track of what face we are computing.
				if (d == 0)
				{
					faceType = backFace ? BlockFaceType::TypeRight : BlockFaceType::TypeLeft;
				}
				else if (d == 1)
				{
					faceType = backFace ? BlockFaceType::TypeTop : BlockFaceType::TypeBottom;
				}
				else if (d == 2)
				{
					faceType = backFace ? BlockFaceType::TypeFront : BlockFaceType::TypeBack;
				}

				// Loop over the entire voxel volume to generate the mask.
				for (x[d] = 0; x[d] < C_SIZE; x[d]++)
				{
					n = 0;

					for (x[v] = 0; x[v] < C_SIZE; x[v]++)
					{
						for (x[u] = 0; x[u] < C_SIZE; x[u]++)
						{
							mask[n++] = IsFaceVisible(x[0], x[1], x[2], faceType) ? 12 : 0; // 12 => GetBlock(x[0], x[1], x[2])->GetType()
						}
					}

					n = 0;

					for (j = 0; j < C_SIZE; j++)
					{
						for (i = 0; i < C_SIZE;)
						{
							// Loop until we find a start point.
							if (mask[n] != 0)
							{
								// Find the width of this mask section, w == current width
								for (w = 1; w + i < C_SIZE && mask[n + w] != 0 && mask[n + w] == mask[n]; w++)
								{
								}

								bool done = false;

								// Find the height of the mask section, h == current height.
								for (h = 1; j + h < C_SIZE; h++)
								{
									// Make sure a whoel row of each height matches the width.
									for (k = 0; k < w; k++)
									{
										if (mask[n + k + h * C_SIZE] == 0 || mask[n + k + h * C_SIZE] != mask[n])
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

								x[u] = i;
								x[v] = j;
								du[0] = 0;
								du[1] = 0;
								du[2] = 0;
								du[u] = w;
								dv[0] = 0;
								dv[1] = 0;
								dv[2] = 0;
								dv[v] = h;

								if (!backFace)
								{
									r = x[0];
									s = x[1];
									t = x[2];
								}
								else
								{
									r = x[0] + q[0];
									s = x[1] + q[1];
									t = x[2] + q[2];
								}

								/*Vector3 vec0 = Vector3(r, s, t);
								Vector3 vec1 = Vector3(r + du[0], s + du[1], t + du[2]);
								Vector3 vec2 = Vector3(r + dv[0], s + dv[1], t + dv[2]);
								Vector3 vec3 = Vector3(r + du[0] + dv[0], s + du[1] + dv[1], t + du[2] + dv[2]);

								// Each face has a specific order of vertices otherwise the textures rotate incorrectly width/height are flipped when dealing with
								// left/right/bottom face due to how rotation of dimensions works, and what order the greedy mesher merges them.
								switch (faceType) {
								case TOP:
									writeQuad(verts, indices, normals, vec1, vec3, vec0, vec2, faceType);
									writeTextureCoords(textCoords, terrain, tmpI.set(x[0], x[1], x[2]), faceType, h, w, MaterialManager.getInstance().getType(mask[n]).getSkin());
									break;
								case BOTTOM:
									writeQuad(verts, indices, normals, vec3, vec1, vec2, vec0, faceType);
									writeTextureCoords(textCoords, terrain, tmpI.set(x[0], x[1], x[2]), faceType, h, w, MaterialManager.getInstance().getType(mask[n]).getSkin());
									break;
								case LEFT:
									writeQuad(verts, indices, normals, vec0, vec2, vec1, vec3, faceType);
									writeTextureCoords(textCoords, terrain, tmpI.set(x[0], x[1], x[2]), faceType, h, w, MaterialManager.getInstance().getType(mask[n]).getSkin());
									break;
								case RIGHT:
									// TODO: figure out why right face overwrites transparency.
									writeQuad(verts, indices, normals, vec2, vec0, vec3, vec1, faceType);
									writeTextureCoords(textCoords, terrain, tmpI.set(x[0], x[1], x[2]), faceType, h, w, MaterialManager.getInstance().getType(mask[n]).getSkin());
									break;
								case FRONT:
									writeQuad(verts, indices, normals, vec0, vec1, vec2, vec3, faceType);
									writeTextureCoords(textCoords, terrain, tmpI.set(x[0], x[1], x[2]), faceType, w, h, MaterialManager.getInstance().getType(mask[n]).getSkin());
									break;
								case BACK:
									writeQuad(verts, indices, normals, vec1, vec0, vec3, vec2, faceType);
									writeTextureCoords(textCoords, terrain, tmpI.set(x[0], x[1], x[2]), faceType, w, h, MaterialManager.getInstance().getType(mask[n]).getSkin());
									break;
								}*/

								unsigned int indexStart = vertices->size();
								BlockFace face = BlockFace(faceType, Vector3(r, s, t), mask[n], Vector3(r + du[0], s + du[1], t + du[2]));
								face.AppendVertices(vertices);
								face.AppendIndices(indices, indexStart);

								// Clear the mask.
								for (l = 0; l < h; ++l)
								{
									for (k = 0; k < w; ++k)
									{
										mask[n + k + l * C_SIZE] = 0;
									}
								}

								// Increment i and n.
								i += w;
								n += w;
							}
							else
							{
								i++;
								n++;
							}
						}
					}
				}
			}
		}
	}
}
