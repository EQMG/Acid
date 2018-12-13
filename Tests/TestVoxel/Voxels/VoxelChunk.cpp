#include "VoxelChunk.hpp"

#include <array>
#include <Objects/GameObject.hpp>
#include <Noise/Noise.hpp>

namespace test
{
	const uint32_t VoxelChunk::CHUNK_WIDTH = 16;
	const uint32_t VoxelChunk::CHUNK_HEIGHT = 16;
	const float VoxelChunk::VOXEL_SIZE = 1.0f;
	const Vector3 VoxelChunk::CHUNK_SIZE = Vector3(VOXEL_SIZE * CHUNK_WIDTH, VOXEL_SIZE * CHUNK_HEIGHT, VOXEL_SIZE * CHUNK_WIDTH);

	VoxelChunk::VoxelChunk(const ChunkMesher &mesher, const bool &generate) :
		IComponent(),
		m_blocks(std::vector<std::vector<std::vector<VoxelBlock>>>(CHUNK_WIDTH, std::vector<std::vector<VoxelBlock>>(CHUNK_WIDTH, std::vector<VoxelBlock>(CHUNK_HEIGHT)))),
		m_mesher(mesher),
		m_generate(generate),
		m_rebuild(true)
	{
		for (uint32_t x = 0; x < CHUNK_WIDTH; x++)
		{
			for (uint32_t z = 0; z < CHUNK_WIDTH; z++)
			{
				for (uint32_t y = 0; y < CHUNK_HEIGHT; y++)
				{
					m_blocks.at(x).at(z).at(y).SetPosition(VOXEL_SIZE * Vector3(x, y, z));
				}
			}
		}
	}

	void VoxelChunk::Start()
	{
	}

	void VoxelChunk::Update()
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

	void VoxelChunk::Decode(const Metadata &metadata)
	{
	}

	void VoxelChunk::Encode(Metadata &metadata) const
	{
	}

	std::optional<VoxelBlock> VoxelChunk::GetBlock(const int32_t &x, const int32_t &y, const int32_t &z)
	{
		if (x >= 0 && x < CHUNK_WIDTH && z >= 0 && z < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT)
		{
			return m_blocks.at(x).at(z).at(y);
		}

		return {};
	}

	void VoxelChunk::SetBlockType(const int32_t &x, const int32_t &y, const int32_t &z, const std::string &type)
	{
		if (x >= 0 && x < CHUNK_WIDTH && z >= 0 && z < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT)
		{
			m_blocks.at(x).at(z).at(y).SetType(type);
			Rebuild();
		}
	}

	bool VoxelChunk::IsBlockFilled(const int32_t &x, const int32_t &y, const int32_t &z)
	{
		auto block = GetBlock(x, y, z);
		return block && !block->GetType().empty();
	}

	bool VoxelChunk::IsFaceVisible(const int32_t &x, const int32_t &y, const int32_t &z, const BlockFace &faceType)
	{
		switch (faceType)
		{
			case FACE_FRONT:
				return !IsBlockFilled(x, y, z - 1);
			case FACE_BACK:
				return !IsBlockFilled(x, y, z + 1);
			case FACE_UP:
				return !IsBlockFilled(x, y + 1, z);
			case FACE_DOWN:
				return !IsBlockFilled(x, y - 1, z);
			case FACE_LEFT:
				return !IsBlockFilled(x - 1, y, z);
			case FACE_RIGHT:
				return !IsBlockFilled(x + 1, y, z);
			default:
				return false;
		}
	}

	void VoxelChunk::Generate()
	{
		auto position = GetGameObject()->GetLocalTransform().GetPosition();
		auto noise = Noise(25653345, 0.01f, NOISE_INTERP_QUINTIC, NOISE_TYPE_PERLIN_FRACTAL, 5, 2.0f, 0.5f, NOISE_FRACTAL_FBM);

		for (uint32_t x = 0; x < CHUNK_WIDTH; x++)
		{
			for (uint32_t z = 0; z < CHUNK_WIDTH; z++)
			{
				for (uint32_t y = 0; y < CHUNK_HEIGHT; y++)
				{
					/*if (x == 8 && y == 0 && z == 8)
					{
						m_blocks.at(x).at(z).at(y).SetType("Stone");
					}*/

					Vector3 blockPosition = (VOXEL_SIZE * Vector3(x, y, z)) + position;

					int height = (int) std::floor(40.0f * noise.GetValueFractal(blockPosition.m_x, blockPosition.m_z));

					if (blockPosition.m_y > height)
					{
						continue;
					}

					if (blockPosition.m_y == height)
					{
						m_blocks.at(x).at(z).at(y).SetType("Grass");
					}
					else if (height - blockPosition.m_y <= 2)
					{
						m_blocks.at(x).at(z).at(y).SetType("Dirt");
					}
					else
					{
						m_blocks.at(x).at(z).at(y).SetType("Stone");
					}
				}
			}
		}
	}

	void VoxelChunk::GenerateMesh()
	{
#if ACID_VERBOSE
		auto debugStart = Engine::GetTime();
#endif

		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr)
		{
			return;
		}

		auto vertices = std::vector<VertexModel>();
		auto indices = std::vector<uint32_t>();

		switch (m_mesher)
		{
			case MESHER_GREEDY:
				CreateGreedyMesh(vertices, indices);
				break;
			case MESHER_CULLED:
				CreateCulledMesh(vertices, indices);
				break;
		}

		if (!vertices.empty() || !indices.empty())
		{
			mesh->SetModel(std::make_shared<Model>(vertices, indices, GetGameObject()->GetName()));
		}
		else
		{
			mesh->SetModel(nullptr);
		}

#if ACID_VERBOSE
		auto debugEnd = Engine::GetTime();

		if ((debugEnd - debugStart).AsMilliseconds() > 22.0f)
		{
			Log::Out("Chunk %s built in %ims\n", GetGameObject()->GetName().c_str(), (debugEnd - debugStart).AsMilliseconds());
		}
#endif
	}

	void VoxelChunk::CreateGreedyMesh(std::vector<VertexModel> &vertices, std::vector<uint32_t> &indices)
	{
		// This method is based off of Robert O'Leary's implementation (https://github.com/roboleary/GreedyMesh)

		// These are just working variables for the algorithm - almost all taken
		// directly from Mikola Lysenko's javascript implementation.
		uint32_t i, j, k, l, w, h, u, v, n;
		BlockFace currentFace;

		auto x = std::array<int32_t, 3>();
		auto q = std::array<uint32_t, 3>();
		auto du = std::array<uint32_t, 3>();
		auto dv = std::array<uint32_t, 3>();

		// We create a mask - this will contain the groups of matching voxel faces
		// as we proceed through the chunk in 6 directions - once for each face.
		auto mask = std::vector<std::string>(CHUNK_WIDTH * CHUNK_HEIGHT);

		// These are just working variables to hold two faces during comparison.
		std::string voxelFace, voxelFace1;

		// We start with the lesser-spotted boolean for-loop (also known as the old flippy floppy).
		// The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows
		// us to track which direction the indices should run during creation of the quad.
		// This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each voxel face.
		for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
		{
			// We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko in his post.
			for (uint32_t d = 0; d < 3; d++)
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
					currentFace = backFace ? FACE_LEFT : FACE_RIGHT;
				}
				else if (d == 1)
				{
					currentFace = backFace ? FACE_DOWN : FACE_UP;
				}
				else if (d == 2)
				{
					currentFace = backFace ? FACE_FRONT : FACE_BACK;
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
							voxelFace = GetVoxelFace(x[0], x[1], x[2], currentFace);
							voxelFace1 = GetVoxelFace(x[0] + q[0], x[1] + q[1], x[2] + q[2], currentFace);

							// We choose the face to add to the mask depending on whether we're moving through on a backface or not.
							mask[n++] = ((!voxelFace.empty() && !voxelFace1.empty() && voxelFace == voxelFace1)) ? "" : backFace ? voxelFace1 : voxelFace;
						}
					}

					x[d]++;

					// Now we generate the mesh for the mask.
					n = 0;

					for (j = 0; j < CHUNK_HEIGHT; j++)
					{
						for (i = 0; i < CHUNK_WIDTH;)
						{
							if (!mask[n].empty())
							{
								// We compute the width.
								for (w = 1; i + w < CHUNK_WIDTH && !mask[n + w].empty() && mask[n + w] == mask[n]; w++)
								{
								}

								// Then we compute height.
								bool done = false;

								for (h = 1; j + h < CHUNK_HEIGHT; h++)
								{
									for (k = 0; k < w; k++)
									{
										if (mask[n + k + h * CHUNK_WIDTH].empty() || mask[n + k + h * CHUNK_WIDTH] != mask[n])
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
									Vector3 bottomLeft = Vector3(x[0], x[1], x[2]);
									Vector3 topLeft = Vector3(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]);
									Vector3 bottomRight = Vector3(x[0] + du[0], x[1] + du[1], x[2] + du[2]);
									Vector3 topRight = Vector3(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]);
									GenerateQuad(vertices, indices, bottomLeft, topLeft, topRight, bottomRight, w, h, mask[n], backFace);
								}

								// We zero out the mask.
								for (l = 0; l < h; ++l)
								{
									for (k = 0; k < w; ++k)
									{
										mask[n + k + l * CHUNK_WIDTH] = "";
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

	void VoxelChunk::CreateCulledMesh(std::vector<VertexModel> &vertices, std::vector<uint32_t> &indices)
	{
		uint32_t u, v;
		BlockFace currentFace;

		auto du = std::array<uint32_t, 3>();
		auto dv = std::array<uint32_t, 3>();

		// We start with the lesser-spotted boolean for-loop (also known as the old flippy floppy).
		for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
		{
			// We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko in his post.
			for (uint32_t d = 0; d < 3; d++)
			{
				u = (d + 1) % 3;
				v = (d + 2) % 3;

				// Here we're keeping track of the side that we're meshing.
				if (d == 0)
				{
					currentFace = backFace ? FACE_LEFT : FACE_RIGHT;
				}
				else if (d == 1)
				{
					currentFace = backFace ? FACE_DOWN : FACE_UP;
				}
				else if (d == 2)
				{
					currentFace = backFace ? FACE_FRONT : FACE_BACK;
				}

				// We move through all of the blocks in the chunk.
				for (uint32_t x = 0; x < CHUNK_WIDTH; x++)
				{
					for (uint32_t z = 0; z < CHUNK_WIDTH; z++)
					{
						for (uint32_t y = 0; y < CHUNK_HEIGHT; y++)
						{
							// Here we filter out invisible faces.
							auto block = GetBlock(x, y, z);

							if (!block || block->GetType().empty())
							{
								continue;
							}

							if (!IsFaceVisible(x, y, z, currentFace))
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
							Vector3 bottomLeft = Vector3(x, y, z);
							Vector3 topLeft = Vector3(x + dv[0], y + dv[1], z + dv[2]);
							Vector3 topRight = Vector3(x + du[0] + dv[0], y + du[1] + dv[1], z + du[2] + dv[2]);
							Vector3 bottomRight = Vector3(x + du[0], y + du[1], z + du[2]);
							GenerateQuad(vertices, indices, bottomLeft, topLeft, topRight, bottomRight, 1, 1, block->GetType(), backFace);
						}
					}
				}
			}
		}
	}

	std::string VoxelChunk::GetVoxelFace(const int32_t &x, const int32_t &y, const int32_t &z, const BlockFace &faceType)
	{
		if (!IsFaceVisible(x, y, z, faceType))
		{
			return "";
		}

		auto block = GetBlock(x, y, z);
		return block ? block->GetType() : "";
	}

	void VoxelChunk::GenerateQuad(std::vector<VertexModel> &vertices, std::vector<uint32_t> &indices,
	                              const Vector3 &bottomLeft, const Vector3 &topLeft, const Vector3 &topRight, const Vector3 &bottomRight,
	                              const int32_t &width, const int32_t &height,
	                              const std::string &blockType, const bool &backFace)
	{
		// Gets where to start indices from.
		uint32_t indexStart = vertices.size();

		Colour colour = VoxelBlock::FindColour(blockType);

		// Calculates the quads normal direction.
		Vector3 normal = (topRight - bottomRight).Cross(topLeft - bottomRight);

		// Flips normal x and z when x is present, I have no clue why.
		if (normal.m_x != 0.0f)
		{
			float tempX = normal.m_x;

			normal.m_x = normal.m_z;
			normal.m_z = tempX;
		}

		normal = normal.Normalize();

		if (backFace)
		{
			normal = normal.Negate();
		}

		// Pushes vertices and indices from quad.
		vertices.emplace_back(VertexModel(VOXEL_SIZE * bottomLeft, Vector2(), normal, colour));
		vertices.emplace_back(VertexModel(VOXEL_SIZE * topLeft, Vector2(), normal, colour));
		vertices.emplace_back(VertexModel(VOXEL_SIZE * bottomRight, Vector2(), normal, colour));
		vertices.emplace_back(VertexModel(VOXEL_SIZE * topRight, Vector2(), normal, colour));

		indices.emplace_back(indexStart + 2);
		indices.emplace_back(indexStart + (backFace ? 0 : 3));
		indices.emplace_back(indexStart + 1);
		indices.emplace_back(indexStart + 1);
		indices.emplace_back(indexStart + (backFace ? 3 : 0));
		indices.emplace_back(indexStart + 2);
	}
}
