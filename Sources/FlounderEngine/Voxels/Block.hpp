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
		Colour *m_colour;
		bool m_filled;

	public:
		Block(Chunk *parent, const Vector3 &position, const Colour &colour, const bool &filled = true);

		~Block();

		void SetChanged();

		Vector3 *GetPosition() const { return m_position; }

		Colour *GetColour() const { return m_colour; }

		bool IsFilled() const { return m_filled; }
	};
}
