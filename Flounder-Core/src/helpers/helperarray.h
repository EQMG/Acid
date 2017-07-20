#pragma once

namespace flounder 
{
	class helperarray 
	{
	public:
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