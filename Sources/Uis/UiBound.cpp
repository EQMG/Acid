#include "UiBound.hpp"

#include "Engine/Log.hpp"

namespace acid
{
	const Vector2 UiBound::TOP_LEFT = Vector2(0.0f, 1.0f);
	const Vector2 UiBound::TOP_CENTRE = Vector2(0.5f, 1.0f);
	const Vector2 UiBound::TOP_RIGHT = Vector2(1.0f, 1.0f);

	const Vector2 UiBound::CENTRE_LEFT = Vector2(0.0f, 0.5f);
	const Vector2 UiBound::CENTRE = Vector2(0.5f, 0.5f);
	const Vector2 UiBound::CENTRE_RIGHT = Vector2(1.0f, 0.5f);

	const Vector2 UiBound::BOTTOM_LEFT = Vector2(0.0f, 0.0f);
	const Vector2 UiBound::BOTTOM_CENTRE = Vector2(0.5f, 0.0f);
	const Vector2 UiBound::BOTTOM_RIGHT = Vector2(1.0f, 0.0f);

	UiBound::UiBound(const Vector2 &position, const Vector2 &reference, const bool &aspectPosition, const bool &aspectSize, const Vector2 &dimensions) :
		m_position(Vector2(position)),
		m_reference(reference),
		m_aspectPosition(aspectPosition),
		m_aspectSize(aspectSize),
		m_dimensions(Vector2(dimensions))
	{
	}

	Vector2 UiBound::GetScreenPosition(const float &aspectRatio) const
	{
		Vector2 screenPosition = m_position;

		if (!m_aspectPosition)
		{
			screenPosition.m_x /= aspectRatio;
		}

		return screenPosition;
	}

	Vector2 UiBound::GetScreenDimensions(const float &aspectRatio) const
	{
		Vector2 screenDimensions = m_dimensions;

		if (m_aspectSize)
		{
			screenDimensions.m_x /= aspectRatio;
		}

		return screenDimensions;
	}

	bool UiBound::operator==(const UiBound &other) const
	{
		return m_position == other.m_position && m_reference == other.m_reference && m_aspectPosition == other.m_aspectPosition && m_aspectSize == other.m_aspectSize && m_dimensions == other.m_dimensions;
	}

	bool UiBound::operator!=(const UiBound &other) const
	{
		return !(*this == other);
	}
}
