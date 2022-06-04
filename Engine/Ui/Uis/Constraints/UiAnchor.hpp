#pragma once

#include "UiExport.hpp"

namespace acid {
class ACID_UI_EXPORT UiAnchor {
public:
	explicit constexpr UiAnchor(float value) : value(value) {}

	constexpr float Get() const { return value; }

	bool operator==(const UiAnchor &rhs) const {
		return value == rhs.value;
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
	float value;
};
}
