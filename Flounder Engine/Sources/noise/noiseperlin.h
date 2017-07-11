#pragma once

#include "../helpers/helperarray.h"

namespace flounder {
	/// <summary>
	/// Computes Perlin Noise for three dimensions.
	/// <p>
	/// The result is a continuous function that interpolates a smooth path along a series random points. The function is consistent, so given the same parameters, it will always return the same value.
	/// <p>
	/// Computing noise for one and two dimensions can make use of the 3D problem flounder.space by just setting the un-needed dimensions to a fixed value.
	/// </summary>
	class noiseperlin
	{
	private:

		int m_seed;
	public:
		noiseperlin(const int seed);

		~noiseperlin();
	};
}
