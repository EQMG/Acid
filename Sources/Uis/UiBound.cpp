#include "UiBound.hpp"

#include "Engine/Log.hpp"

namespace acid
{
	const Vector2 UiBound::TopLeft = Vector2(0.0f, 0.0f);
	const Vector2 UiBound::TopCentre = Vector2(0.5f, 0.0f);
	const Vector2 UiBound::TopRight = Vector2(1.0f, 0.0f);
	const Vector2 UiBound::CentreLeft = Vector2(0.0f, 0.5f);
	const Vector2 UiBound::Centre = Vector2(0.5f, 0.5f);
	const Vector2 UiBound::CentreRight = Vector2(1.0f, 0.5f);
	const Vector2 UiBound::BottomLeft = Vector2(0.0f, 1.0f);
	const Vector2 UiBound::BottomCentre = Vector2(0.5f, 1.0f);
	const Vector2 UiBound::BottomRight = Vector2(1.0f, 1.0f);

	const UiBound UiBound::Screen = UiBound(Vector2(0.0f, 0.0f), TopLeft, true, false, Vector2(1.0f, 1.0f));

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
