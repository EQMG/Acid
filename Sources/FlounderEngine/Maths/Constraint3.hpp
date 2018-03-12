#pragma once

#include <ostream>
#include <string>
#include "../Prerequisites.hpp"
#include "../Files/LoadedValue.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 3-tuple vector.
	/// </summary>
	class F_EXPORT Constraint3
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

		static const Constraint3 FALSE;
		static const Constraint3 TRUE;

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
		/// Constructor for Constraint3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a loaded value. </param>
		Constraint3(LoadedValue *value);

		/// <summary>
		/// Deconstructor for Constraint3.
		/// </summary>
		~Constraint3();

		/// <summary>
		/// Loads from another Constraint3.
		/// </summary>
		/// <param name="source"> The source constraint. </param>
		/// <returns> This. </returns>
		Constraint3 *Set(const Constraint3 &source);

		/// <summary>
		/// Sets values in the constraint.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <returns> This. </returns>
		Constraint3 *Set(const bool &x, const bool &y, const bool &z);

		/// <summary>
		/// Sets values in the constraint.
		/// </summary>
		/// <param name="source"> The source loaded value. </param>
		Constraint3 *Set(LoadedValue *value);

		/// <summary>
		/// Saves this constraint into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		Constraint3 &operator=(const Constraint3 &other);

		bool operator==(const Constraint3 &other) const;

		bool operator!=(const Constraint3 &other) const;

		friend std::ostream &operator<<(std::ostream &stream, const Constraint3 &constraint);

		std::string ToString() const;
	};
}
