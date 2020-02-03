#pragma once

namespace acid {
class UiAnchor {
public:
	explicit constexpr UiAnchor(float value) : m_value(value) {}
	constexpr float Get() const { return m_value; }

	bool operator==(const UiAnchor &rhs) const {
		return m_value == rhs.m_value;
	}

	bool operator!=(const UiAnchor &rhs) const {
		return !operator==(rhs);
	}

	static const UiAnchor Zero;
	static const UiAnchor Left;
	static const UiAnchor Top;
	static const UiAnchor Centre;
	static const UiAnchor Right;
	static const UiAnchor Bottom;

private:
	float m_value;
};

inline constexpr UiAnchor UiAnchor::Zero(0.0f);
inline constexpr UiAnchor UiAnchor::Left(0.0f);
inline constexpr UiAnchor UiAnchor::Top(0.0f);
inline constexpr UiAnchor UiAnchor::Centre(0.5f);
inline constexpr UiAnchor UiAnchor::Right(1.0f);
inline constexpr UiAnchor UiAnchor::Bottom(1.0f);
}
