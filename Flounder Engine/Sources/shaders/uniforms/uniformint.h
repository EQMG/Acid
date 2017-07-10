#pragma once

#include "iuniform.h"

namespace flounder {
	/// <summary>
	/// Represents a int uniform type that can be loaded to the shader.
	/// </summary>
	class uniformint : public iuniform
	{
	private:
		int current;
	public:
		uniformint(const std::string &name, shader *shader);

		~uniformint();

		/// <summary>
		/// Loads a float to the uniform if the value already on the GPU is not the same as the new value.
		/// </summary>
		/// <param name="value"> The new value. </param>
		void loadInt(int value);
	};
}
