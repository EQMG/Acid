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
		short m_type;
		bool m_filled;

	public:
		Block(Chunk *parent, const Vector3 &position, const short &type, const bool &filled = true);

		~Block();

		void SetChanged();

		Vector3 *GetPosition() const { return m_position; }

		short GetType() const { return m_type; }

		void SetType(const short &type) { m_type = type; }

		bool IsFilled() const { return m_filled; }
	};
}
