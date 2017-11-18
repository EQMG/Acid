#include "HelperArray.hpp"

namespace Flounder
{
	float **HelperArray::FloatArray(const int &size1, const int &size2)
	{
		float **array = new float*[size1];

		for (int i = 0; i < size1; i++)
		{
			array[i] = new float[size2];
		}

		return array;
	}

	Vector3 **HelperArray::Vector3Array(const int &size1, const int &size2)
	{
		Vector3 **array = new Vector3*[size1];

		for (int i = 0; i < size1; i++)
		{
			array[i] = new Vector3[size2];
		}

		return array;
	}
}
