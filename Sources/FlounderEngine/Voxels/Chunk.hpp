#pragma once

#include "../Objects/Component.hpp"
#include "../Meshes/Mesh.hpp"
#include "Block.hpp"
#include "BlockFace.hpp"

namespace Flounder
{
	typedef std::vector<std::vector<std::vector<Block*>>> BlockVector;

	class F_EXPORT Chunk :
		public Component
	{
	private:
		BlockVector *m_blocks;
		bool m_generate;
		bool m_rebuild;
	public:
		static const unsigned int SIDE_LENGTH;

		Chunk(const bool &generate = true);

		~Chunk();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Chunk"; };

		Block *GetBlock(const unsigned int &x, const unsigned int &y, const unsigned int &z);

		bool IsBlockFilled(const unsigned int &x, const unsigned int &y, const unsigned int &z);

		void Rebuild() { m_rebuild = true; }

	private:
		void Generate();

		void GenerateMesh();

		void SearchFaces(const unsigned int &x, const unsigned int &y, const unsigned int &z, Colour *colour, std::vector<BlockFace> *faces);
	};
}
