#pragma once

#include <map>
#include <Maths/Colour.hpp>
#include <Maths/Vector3.hpp>

using namespace acid;

namespace test
{
	class VoxelChunk;

	enum BlockFace
	{
		FACE_FRONT = 0,
		FACE_BACK = 1,
		FACE_UP = 2,
		FACE_DOWN = 3,
		FACE_LEFT = 4,
		FACE_RIGHT = 5
	};

	class VoxelBlock
	{
	private:
		static std::map<std::string, Colour> BLOCK_COLOURS;

		Vector3 m_position;
		std::string m_type;
	public:
		explicit VoxelBlock(const Vector3 &position = Vector3::Zero, const std::string &type = "");

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { m_position = position; }

		std::string GetType() const { return m_type; }

		void SetType(const std::string &type) { m_type = type; }

		static Colour FindColour(const std::string &key);

		static void AddColour(const std::string &key, const Colour &colour);
	};
}
