#pragma once

#include "Helpers/NonCopyable.hpp"
#include "StdAfx.hpp"

namespace acid
{
/**
 * A interface used for defining engine modules.
 */
class ACID_EXPORT Module :
	public NonCopyable
{
public:
	/**
	 * Represents when a module will call <seealso cref="Module#Update()"/> in the update loop.
	 */
	enum class Stage
	{
		Always, Pre, Normal, Post, Render
	};

	Module() = default;

	virtual ~Module() = default;

	/**
	 * The update function for the module.
	 */
	virtual void Update() = 0;
};
}
