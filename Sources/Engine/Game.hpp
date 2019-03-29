#pragma once

#include "StdAfx.hpp"

namespace acid
{
/// <summary>
/// A class that is used to represent a game.
/// </summary>
class ACID_EXPORT Game
{
public:
	Game() :
		m_started(false)
	{
	}

	virtual ~Game() = default;

	/// <summary>
	/// Run when starting the game if <seealso cref="#m_started"/> is false.
	/// </summary>
	virtual void Start() = 0;

	/// <summary>
	/// Run before the module update pass.
	/// </summary>
	virtual void Update() = 0;

private:
	friend class Engine;
	bool m_started;
};
}
