#include "Chunk.hpp"

#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	const int Chunk::CHUNK_WIDTH = 16;
	const int Chunk::CHUNK_HEIGHT = 16;
	const Vector3 *Chunk::CHUNK_SIZE = new Vector3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);

	Chunk::Chunk(const ChunkMesh &chunkMesh, const bool &generate) :
		Component(),
		m_chunkMesh(chunkMesh),
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

	bool Chunk::IsFaceVisible(const unsigned int &x, const unsigned int &y, const unsigned int &z, const FaceSide &faceType)
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

					Block *block = new Block(this, 2.0f * Vector3(x, y, z), 1);
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

		switch (m_chunkMesh)
		{
		case MeshGreedy:
			CreateGreedyMesh(&vertices, &indices);
			break;
		case MeshSimple:
			CreateSimpleMesh(&vertices, &indices);
			break;
		}

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
		int u, v;
		FaceSide currentFace;

		std::vector<int> du = std::vector<int>{0, 0, 0};
		std::vector<int> dv = std::vector<int>{0, 0, 0};

		// We start with the lesser-spotted boolean for-loop (also known as the old flippy floppy).
		for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
		{
			// We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko in his post.
			for (int d = 0; d < 3; d++)
			{
				u = (d + 1) % 3;
				v = (d + 2) % 3;

				// Here we're keeping track of the side that we're meshing.
				if (d == 0)
				{
					currentFace = backFace ? FaceLeft : FaceRight;
				}
				else if (d == 1)
				{
					currentFace = backFace ? FaceBottom : FaceTop;
				}
				else if (d == 2)
				{
					currentFace = backFace ? FaceFront : FaceBack;
				}

				// We move through all of the blocks in the chunk.
				for (int i = 0; i < CHUNK_WIDTH; i++)
				{
					for (int k = 0; k < CHUNK_WIDTH; k++)
					{
						for (int j = 0; j < CHUNK_HEIGHT; j++)
						{
							// Here we filter out invisible faces.
							if (!IsFaceVisible(i, j, k, currentFace))
							{
								continue;
							}

							Block *block = GetBlock(i, j, k);

							if (block == nullptr || !block->IsFilled())
							{
								continue;
							}

							du[0] = 0;
							du[1] = 0;
							du[2] = 0;
							du[u] = 1;

							dv[0] = 0;
							dv[1] = 0;
							dv[2] = 0;
							dv[v] = 1;

							// And here we call the quad function in order to render a merged quad in the scene.
							GenerateQuad(vertices, indices, block->GetType(), currentFace, backFace, 1, 1, {i, j, k}, du, dv);
						}
					}
				}
			}
		}
	}

	void Chunk::CreateGreedyMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices)
	{
		// This method is based off of Robert O'Leary's implementation (https://github.com/roboleary/GreedyMesh)

        // These are just working variables for the algorithm - almost all taken
		// directly from Mikola Lysenko's javascript implementation.
		int i, j, k, l, w, h, u, v, n;
		FaceSide currentFace;

		std::vector<int> x = std::vector<int>{0, 0, 0};
		std::vector<int> q = std::vector<int>{0, 0, 0};
		std::vector<int> du = std::vector<int>{0, 0, 0};
		std::vector<int> dv = std::vector<int>{0, 0, 0};

		// We create a mask - this will contain the groups of matching voxel faces
		// as we proceed through the chunk in 6 directions - once for each face.
		std::vector<short> mask = std::vector<short>(CHUNK_WIDTH * CHUNK_HEIGHT);

 		// These are just working variables to hold two faces during comparison.
		short voxelFace, voxelFace1;

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
					currentFace = backFace ? FaceLeft : FaceRight;
				}
				else if (d == 1)
				{
					currentFace = backFace ? FaceBottom : FaceTop;
				}
				else if (d == 2)
				{
					currentFace = backFace ? FaceFront : FaceBack;
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
							voxelFace = (x[d] >= 0 ) ? GetVoxelFace(x[0], x[1], x[2], currentFace) : (short)0;
							voxelFace1 = (x[d] < CHUNK_WIDTH - 1) ? GetVoxelFace(x[0] + q[0], x[1] + q[1], x[2] + q[2], currentFace) : (short)0;

							// We choose the face to add to the mask depending on whether we're moving through on a backface or not.
							mask[n++] = ((voxelFace != 0 && voxelFace1 != 0 && voxelFace == voxelFace1))
								? (short)0
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
							if (mask[n] != 0)
							{
								// We compute the width.
								for (w = 1; i + w < CHUNK_WIDTH && mask[n + w] != 0 && mask[n + w] == mask[n]; w++)
								{
								}

								// Then we compute height.
								bool done = false;

								for (h = 1; j + h < CHUNK_HEIGHT; h++)
								{
									for (k = 0; k < w; k++)
									{
										if (mask[n + k + h * CHUNK_WIDTH] == 0 || mask[n + k + h * CHUNK_WIDTH] != mask[n])
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
									GenerateQuad(vertices, indices, mask[n], currentFace, backFace, w, h, x, du, dv);
								}

								// We zero out the mask.
								for (l = 0; l < h; ++l)
								{
									for(k = 0; k < w; ++k)
									{
										mask[n + k + l * CHUNK_WIDTH] = 0;
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

	short Chunk::GetVoxelFace(const int &x, const int &y, const int &z, const FaceSide &faceType)
	{
	//	if (!IsFaceVisible(x, y, z, faceType))
	//	{
	//		return 0;
	//	}

		Block *block = GetBlock(x, y, z);
		return block != nullptr ? block->GetType() : (short)0;
	}

	void Chunk::GenerateQuad(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices,
							 const short &mask, const FaceSide &faceSide, const bool &backFace,
							 const int &width, const int &height,
							 const std::vector<int> &x,
							 const std::vector<int> &du,
							 const std::vector<int> &dv)
	{
		unsigned int indexStart = vertices->size();

		Colour colour = *BlockFace::FindColour(mask);
		Vector3 normal = Vector3();

		switch (faceSide)
		{
		case FaceFront:
			normal.Set(0.0f, 0.0f, -1.0f);
			break;
		case FaceBack:
			normal.Set(0.0f, 0.0f, 1.0f);
			break;
		case FaceTop:
			normal.Set(0.0f, 1.0f, 0.0f);
			break;
		case FaceBottom:
			normal.Set(0.0f, -1.0f, 0.0f);
			break;
		case FaceLeft:
			normal.Set(0.0f, 0.0f, -1.0f);
			break;
		case FaceRight:
			normal.Set(0.0f, 0.0f, 1.0f);
			break;
		}

		vertices->push_back(Vertex(2.0f * Vector3(x[0], x[1], x[2]), Vector2(), normal, colour));
		vertices->push_back(Vertex(2.0f * Vector3(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]), Vector2(), normal, colour));
		vertices->push_back(Vertex(2.0f * Vector3(x[0] + du[0], x[1] + du[1], x[2] + du[2]), Vector2(), normal, colour));
		vertices->push_back(Vertex(2.0f * Vector3(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]), Vector2(), normal, colour));

		if (backFace)
		{
			indices->push_back(indexStart + 2);
			indices->push_back(indexStart + 0);
			indices->push_back(indexStart + 1);
			indices->push_back(indexStart + 1);
			indices->push_back(indexStart + 3);
			indices->push_back(indexStart + 2);
		}
		else
		{
			indices->push_back(indexStart + 2);
			indices->push_back(indexStart + 3);
			indices->push_back(indexStart + 1);
			indices->push_back(indexStart + 1);
			indices->push_back(indexStart + 0);
			indices->push_back(indexStart + 2);
		}
	}
}
