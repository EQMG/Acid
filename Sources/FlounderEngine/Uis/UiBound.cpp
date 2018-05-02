#include "UiBound.hpp"

namespace fl
{
	std::map<std::string, Vector2> UiBound::PIVOT_MAP = std::map<std::string, Vector2>
		{
			{"TopLeft",      Vector2(0.0f, 1.0f)},
			{"TopCentre",    Vector2(0.5f, 1.0f)},
			{"TopRight",     Vector2(1.0f, 1.0f)},

			{"CentreLeft",   Vector2(0.0f, 0.5f)},
			{"Centre",       Vector2(0.5f, 0.5f)},
			{"CentreRight",  Vector2(1.0f, 0.5f)},

			{"BottomLeft",   Vector2(0.0f, 0.0f)},
			{"BottomCentre", Vector2(0.5f, 0.0f)},
			{"BottomRight",  Vector2(1.0f, 0.0f)},
		};

	UiBound::UiBound(const Vector2 &position, const std::string &reference, const bool &aspectPosition, const bool &aspectSize, const Vector2 &dimensions) :
		m_position(new Vector2(position)),
		m_reference(new Vector2(FindPivot(reference))),
		m_aspectPosition(aspectPosition),
		m_aspectSize(aspectSize),
		m_dimensions(new Vector2(dimensions))
	{
	}

	UiBound::UiBound(const UiBound &source) :
		m_position(new Vector2(*source.m_position)),
		m_reference(new Vector2(*source.m_reference)),
		m_aspectPosition(source.m_aspectPosition),
		m_aspectSize(source.m_aspectSize),
		m_dimensions(new Vector2(*source.m_dimensions))
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
		*m_position = *source.m_position;
		*m_reference = *source.m_reference;
		m_aspectPosition = source.m_aspectPosition;
		m_aspectSize = source.m_aspectSize;
		*m_dimensions = *source.m_dimensions;
		return this;
	}

	Vector2 UiBound::FindPivot(const std::string &key)
	{
		const auto it = PIVOT_MAP.find(key);

		if (it == PIVOT_MAP.end())
		{
#if FL_VERBOSE
			printf("Could not find a UiBound pivot from key: %s", key.c_str());
#endif
			return Vector2(0.0f, 1.0f);
		}

		return it->second;
	}
}
