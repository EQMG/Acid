#pragma once

namespace Flounder
{
	/// <summary>
	/// A helper for C++ arrays.
	/// </summary>
	class helperarray
	{
	public:
		/// <summary>
		/// Creates a new rectangular float array.
		/// </summary>
		/// <param name="size1"> The first size. </param>
		/// <param name="size2"> The seconds size. </param>
		/// <returns> The new rectangular float array. </returns>
		static float **recFloatArray(const int &size1, const int &size2)
		{
			float **array = new float*[size1];

			for (int i = 0; i < size1; i++)
			{
				array[i] = new float[size2];
			}

			return array;
		}
	};
}
