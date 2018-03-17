#pragma once

#include "../Maths/Colour.hpp"
#include "../Maths/Vector3.hpp"

namespace Flounder
{
	class Chunk;

	class F_EXPORT Block
	{
	private:
		Chunk *m_parent;

		Vector3 *m_position;
		std::string m_type;

	public:
		Block(Chunk *parent, const Vector3 &position, const std::string &type);

		~Block();

		void SetChanged();

		Vector3 *GetPosition() const { return m_position; }

		std::string GetType() const { return m_type; }

		void SetType(const std::string &type) { m_type = type; }
	};
}
