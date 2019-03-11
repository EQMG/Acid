#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Pch.hpp"

namespace acid
{
	/// <summary>
	/// A interface used for defining engine modules.
	/// </summary>
	class ACID_EXPORT Module :
		public NonCopyable
	{
	public:
		/// <summary>
		/// Represents when a module will call <seealso cref="Module#Update()"/> in the update loop.
		/// </summary>
		enum class Stage
		{
			Always, Pre, Normal, Post, Render
		};

		Module() = default;

		virtual ~Module() = default;

		/// <summary>
		/// The update function for the module.
		/// </summary>
		virtual void Update() = 0;
	};
}
