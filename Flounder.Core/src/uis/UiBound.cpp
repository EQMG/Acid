#include "UiBound.hpp"

namespace Flounder
{
	std::map<std::string, Vector2> UiBound::g_pivotMap = std::map<std::string, Vector2>
	{
		{ "TopLeft", Vector2(0.0f, 1.0f) },
		{ "TopCentre", Vector2(0.5f, 1.0f) },
		{ "TopRight", Vector2(1.0f, 1.0f) },

		{ "CentreLeft", Vector2(0.0f, 0.5f) },
		{ "Centre", Vector2(0.5f, 0.5f) },
		{ "CentreRight", Vector2(1.0f, 0.5f) },

		{ "ButtomLeft", Vector2(0.0f, 0.0f) },
		{ "ButtomCentre", Vector2(0.5f, 0.0f) },
		{ "ButtomRight", Vector2(1.0f, 0.0f) },
	};

	UiBound::UiBound(const Vector2 &position, const Vector2 &dimensions, const int &flagsAspect, const Vector2 &reference) :
		m_position(new Vector2(position)),
		m_dimensions(new Vector2(dimensions)),
		m_flagsAspect(flagsAspect),
		m_reference(new Vector2(reference))
	{
	}

	UiBound::UiBound(const UiBound &source) : 
		m_position(new Vector2(*source.m_position)),
		m_dimensions(new Vector2(*source.m_dimensions)),
		m_flagsAspect(source.m_flagsAspect),
		m_reference(new Vector2(*source.m_reference))
	{
	}

	UiBound::~UiBound()
	{
		delete m_position;
		delete m_dimensions;
		delete m_reference;
	}

	UiBound *UiBound::Set(const UiBound &source)
	{
		m_position->Set(*source.m_position);
		m_dimensions->Set(*source.m_dimensions);
		m_flagsAspect = source.m_flagsAspect;
		m_reference->Set(*source.m_reference);
		return this;
	}

	Vector2 UiBound::FindPivot(const std::string &key)
	{
		const auto it = g_pivotMap.find(key);

		if (it == g_pivotMap.end())
		{
#if FLOUNDER_VERBOSE
			printf("Could not find a UiBound pivot from key: %s", key.c_str());
#endif
			return Vector2(0.0f, 1.0f);
		}

		return it->second;
	}
}
