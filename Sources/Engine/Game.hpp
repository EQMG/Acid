#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief Class that represents a game.
 */
class ACID_EXPORT Game
{
public:
	Game() :
		m_started(false)
	{
	}

	virtual ~Game() = default;

	/**
	 * Run when starting the game if {@link Game#m_started} is false.
	 */
	virtual void Start() = 0;

	/**
	 * Run before the module update pass.
	 */
	virtual void Update() = 0;

private:
	friend class Engine;
	bool m_started;
};
}
