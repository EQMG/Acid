#include "SkinLoader.hpp"

namespace Flounder
{
	SkinLoader::SkinLoader(LoadedValue *libraryControllers, const int &maxWeights) :
		m_libraryControllers(libraryControllers),
		m_maxWeights(maxWeights),
		m_skinningData(nullptr)
	{
	}

	SkinLoader::~SkinLoader()
	{
		delete m_skinningData;
	}
}
