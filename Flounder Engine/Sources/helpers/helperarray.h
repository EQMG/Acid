#pragma once

namespace flounder {
	class helperarray 
	{
	public:
		static float** recFloatArray(int size1, int size2)
		{
			float** array = new float*[size1];
			
			for (int array1 = 0; array1 < size1; array1++)
			{
				array[array1] = new float[size2];
			}

			return array;
		}
	};
}