#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief Class that represents a game.
 */
class Game
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
