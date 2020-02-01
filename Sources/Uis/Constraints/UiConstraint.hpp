#pragma once

#include <cstdint>

namespace acid {
class UiConstraints;

enum class UiConstraintType {
	X, Y, Width, Height
};

template<UiConstraintType Type>
class UiConstraint {
public:
	virtual ~UiConstraint() = default;

	bool Update(const UiConstraints *object, const UiConstraints *parent) {
		auto last = m_current;
		m_current = Calculate(object, parent) + m_offset;
		return m_current != last;
	}

	virtual int32_t Calculate(const UiConstraints *object, const UiConstraints *parent) const = 0;

	/**
	 * Gets the constraints value.
	 * @return The current value.
	 */
	virtual int32_t Get() const { return m_current; }

	int32_t GetOffset() const { return m_offset; }
	void SetOffset(int32_t offset) { m_offset = offset; }

protected:
	/// The most recent value calculation.
	int32_t m_current = 0;
	/// Value offset in pixels.
	int32_t m_offset = 0;
};
}

