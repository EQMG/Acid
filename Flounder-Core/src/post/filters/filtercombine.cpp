#include "filtercombine.h"

namespace flounder
{
	filtercombine::filtercombine() :
#ifdef FLOUNDER_API_WEB
		ipostfilter("filterCombine", "res/shaders/filters/combineFragment.web.glsl")
#else
		ipostfilter("filterCombine", "res/shaders/filters/combineFragment.glsl")
#endif
	{
		m_slideSpaceValue = new vector4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	filtercombine::~filtercombine()
	{
		delete m_slideSpaceValue;
	}

	void filtercombine::storeValues()
	{
		// TODO: Load and store as seperate floats.
		m_shader->loadUniform4f("slideSpace", *m_slideSpaceValue);
	}
}
