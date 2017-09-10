#include "FilterCombine.hpp"

namespace Flounder
{
	FilterCombine::FilterCombine() :
		IPostFilter("filterCombine", "res/shaders/filters/combine.frag.spv"),
		m_slideSpaceValue(new Vector4(0.0f, 1.0f, 0.0f, 1.0f))
	{
	}

	FilterCombine::~FilterCombine()
	{
		delete m_slideSpaceValue;
	}

	void FilterCombine::StoreValues()
	{
#if 0
		// TODO: Load and store as seperate floats.
		m_shader->loadUniform4f("slideSpace", *m_slideSpaceValue);
#endif
	}
}
