#include "uniformint.h"

namespace flounder {
	uniformint::uniformint(const std::string &name, shader *shader)
		: iuniform(name, shader)
	{
	}

	uniformint::~uniformint()
	{
	}

	void uniformint::loadInt(int value)
	{
		if (current != value)
		{
			current = value;
			glUniform1i(getLocation(), value);
		}
	}
}
