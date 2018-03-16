#include "Chunk.hpp"

#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	const int Chunk::CHUNK_WIDTH = 16;
	const int Chunk::CHUNK_HEIGHT = 16;
	const Vector3 *Chunk::CHUNK_SIZE = new Vector3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);

	Chunk::Chunk(const bool &generate) :
		Component(),
		m_position(new Vector3()),
		m_blocks(new BlockVector()),
		m_generate(generate),
		m_rebuild(true)
	{
		for (unsigned int x = 0; x < CHUNK_WIDTH; x++)
		{
			m_blocks->push_back(std::vector<std::vector<Block*>>());

			for (unsigned int z = 0; z < CHUNK_WIDTH; z++)
			{
				m_blocks->at(x).push_back(std::vector<Block*>());

				for (unsigned int y = 0; y < CHUNK_HEIGHT; y++)
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
		if (x < 0 || x >= CHUNK_WIDTH || z < 0 || z >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT)
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

	bool Chunk::IsFaceVisible(const unsigned int &x, const unsigned int &y, const unsigned int &z, const FaceType &faceType)
	{
		switch (faceType)
		{
		case FaceFront:
			return IsBlockFilled(x, y, z - 1);
		case FaceBack:
			return IsBlockFilled(x, y, z + 1);
		case FaceTop:
			return IsBlockFilled(x, y + 1, z);
		case FaceBottom:
			return IsBlockFilled(x, y - 1, z);
		case FaceLeft:
			return IsBlockFilled(x - 1, y, z);
		case FaceRight:
			return IsBlockFilled(x + 1, y, z);
		default:
			return false;
		}
	}

	void Chunk::Generate()
	{
		*m_position = *GetGameObject()->GetTransform()->GetPosition();
		auto position = *GetGameObject()->GetTransform()->GetPosition() / 2.0f;
		auto noise = Worlds::Get()->GetNoise();

		for (unsigned int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (unsigned int z = 0; z < CHUNK_WIDTH; z++)
			{
				for (unsigned int y = 0; y < CHUNK_HEIGHT; y++)
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

	//	CreateSimpleMesh(&vertices, &indices);
		CreateGreedyMesh(&vertices, &indices);

		delete mesh->GetModel();

		if (!vertices.empty() || !indices.empty())
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
		for (unsigned int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (unsigned int z = 0; z < CHUNK_WIDTH; z++)
			{
				for (unsigned int y = 0; y < CHUNK_HEIGHT; y++)
				{
					Block *block = GetBlock(x, y, z);

					if (block == nullptr || !block->IsFilled())
					{
						continue;
					}

					Vector3 position = 2.0f * Vector3(x, y, z);

					for (unsigned int faceType = 1; faceType < 7; faceType++)
					{
						if (IsFaceVisible(x, y, z, (FaceType)faceType))
						{
							BlockFace face = BlockFace((FaceType)faceType, block->GetType());

							unsigned int indexStart = vertices->size();

							face.AppendVertices(vertices, position, Vector3::ONE);
							face.AppendIndices(indices, indexStart);
						}
					}
				}
			}
		}
	}

	void Chunk::CreateGreedyMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices)
	{
        // These are just working variables for the algorithm - almost all taken
		// directly from Mikola Lysenko's javascript implementation.
		int i, j, k, l, w, h, u, v, n;
		FaceType side;

		std::vector<int> x = std::vector<int>{0, 0, 0};
		std::vector<int> q = std::vector<int>{0, 0, 0};
		std::vector<int> du = std::vector<int>{0, 0, 0};
		std::vector<int> dv = std::vector<int>{0, 0, 0};

		// We create a mask - this will contain the groups of matching voxel faces
		// as we proceed through the chunk in 6 directions - once for each face.
		std::vector<BlockFace*> mask = std::vector<BlockFace*>(CHUNK_WIDTH * CHUNK_HEIGHT);

 		// These are just working variables to hold two faces during comparison.
		BlockFace *voxelFace = nullptr;
		BlockFace *voxelFace1 = nullptr;

		// We start with the lesser-spotted boolean for-loop (also known as the old flippy floppy).
		// The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows
		// us to track which direction the indices should run during creation of the quad.
		// This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each voxel face.
		for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
		{
			// We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko in his post.
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

				// Here we're keeping track of the side that we're meshing.
				if (d == 0)
				{
					side = backFace ? FaceLeft : FaceRight;
				}
				else if (d == 1)
				{
					side = backFace ? FaceBottom : FaceTop;
				}
				else if (d == 2)
				{
					side = backFace ? FaceBack : FaceFront;
				}

				// We move through the dimension from front to back.
				for (x[d] = -1; x[d] < CHUNK_WIDTH;)
				{
					// We compute the mask.
					n = 0;

					for (x[v] = 0; x[v] < CHUNK_HEIGHT; x[v]++)
					{
						for (x[u] = 0; x[u] < CHUNK_WIDTH; x[u]++)
						{
							// Here we retrieve two voxel faces for comparison.
							voxelFace = (x[d] >= 0 ) ? GetVoxelFace(x[0], x[1], x[2], side) : nullptr;
							voxelFace1 = (x[d] < CHUNK_WIDTH - 1) ? GetVoxelFace(x[0] + q[0], x[1] + q[1], x[2] + q[2], side) : nullptr;

							// We choose the face to add to the mask depending on whether we're moving through on a backface or not.
							mask[n++] = ((voxelFace != nullptr && voxelFace1 != nullptr && voxelFace == voxelFace1))
								? nullptr
								: backFace ? voxelFace1 : voxelFace;
						}
					}

					x[d]++;

					// Now we generate the mesh for the mask.
					n = 0;

					for (j = 0; j < CHUNK_HEIGHT; j++)
					{
						for (i = 0; i < CHUNK_WIDTH;)
						{
							if (mask[n] != nullptr)
							{
								// We compute the width.
								for (w = 1; i + w < CHUNK_WIDTH && mask[n + w] != nullptr && mask[n + w] == mask[n]; w++)
								{
								}

								// Then we compute height.
								bool done = false;

								for (h = 1; j + h < CHUNK_HEIGHT; h++)
								{
									for (k = 0; k < w; k++)
									{
										if (mask[n + k + h * CHUNK_WIDTH] == nullptr || mask[n + k + h * CHUNK_WIDTH] != mask[n])
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

								// Here we check if the BlockFace is transparent, we don't mesh any culled faces.
							//	if (!mask[n].transparent)
								{
									// Add quad.
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

									// And here we call the quad function in order to render a merged quad in the scene.
									unsigned int indexStart = vertices->size();
									mask[n]->AppendVertices(vertices, 2.0f * Vector3(x[0], x[1], x[2]), Vector3(w, h, 1.0f));
									mask[n]->AppendIndices(indices, indexStart);
								}

								// We zero out the mask.
								for (l = 0; l < h; ++l)
								{
									for(k = 0; k < w; ++k)
									{
										mask[n + k + l * CHUNK_WIDTH] = nullptr;
									}
								}

								// And then finally increment the counters and continue.
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

	BlockFace Chunk::GetVoxelFace(const int &x, const int &y, const int &z, const FaceType &faceType)
	{
		// return IsFaceVisible(x, y, z, faceType) ? faceType : nullptr;
		auto block = GetBlock(x, y, z);
		return BlockFace(faceType, block == nullptr ? 0 : block->GetType());
	}
}
