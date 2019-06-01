#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief class  that represents a game.
 */
class ACID_EXPORT Game
{
public:
	virtual ~Game() = default;

	/**
	 * Run before the module update pass.
	 */
	virtual void Update() = 0;

private:
	friend class Engine;
};
}
