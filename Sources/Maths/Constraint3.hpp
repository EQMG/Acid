#pragma once

#include <ostream>
#include <string>
#include "Engine/Exports.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	/// <summary>
	/// Holds a 3-tuple vector.
	/// </summary>
	class FL_EXPORT Constraint3
	{
	public:
		union
		{
			struct
			{
				bool m_x, m_y, m_z;
			};

			struct
			{
				bool m_elements[3];
			};
		};

	//	static const Constraint3 FALSE;
	//	static const Constraint3 TRUE;

		/// <summary>
		/// Constructor for Constraint3.
		/// </summary>
		Constraint3();

		/// <summary>
		/// Constructor for Constraint3.
		/// </summary>
		/// <param name="source"> Creates this constraint out of a existing one. </param>
		Constraint3(const Constraint3 &source);

		/// <summary>
		/// Constructor for Constraint3.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		Constraint3(const bool &x, const bool &y, const bool &z);

		/// <summary>
		/// Deconstructor for Constraint3.
		/// </summary>
		~Constraint3();

		/// <summary>
		/// Saves this constraint into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *source);

		bool GetX() const { return m_x; }

		void SetX(const bool &x) { m_x = x; }

		bool GetY() const { return m_y; }

		void SetY(const bool &y) { m_y = y; }

		bool GetZ() const { return m_z; }

		void SetZ(const bool &z) { m_z = z; }

		Constraint3 &operator=(const Constraint3 &other);

		Constraint3 &operator=(LoadedValue *value);

		bool operator==(const Constraint3 &other) const;

		bool operator!=(const Constraint3 &other) const;

		FL_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Constraint3 &constraint);

		std::string ToString() const;
	};
}
