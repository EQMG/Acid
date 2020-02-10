#pragma once

#include <memory>

#include "Helpers/NonCopyable.hpp"
#include "UiConstraint.hpp"

namespace acid {
class ACID_EXPORT UiConstraints final : NonCopyable {
public:
	UiConstraints();

	bool Update(const UiConstraints *parent);

	UiConstraint<UiConstraintType::X> *GetX() const { return m_x.get(); }
	UiConstraint<UiConstraintType::Y> *GetY() const { return m_y.get(); }
	UiConstraint<UiConstraintType::Width> *GetWidth() const { return m_width.get(); }
	UiConstraint<UiConstraintType::Height> *GetHeight() const { return m_height.get(); }

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::X> *, UiConstraint<UiConstraintType::X> *>>>
		UiConstraints &SetX(Args &&... args) {
		m_x = std::make_unique<T<UiConstraintType::X>>(std::forward<Args>(args)...);
		return *this;
	}

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::Y> *, UiConstraint<UiConstraintType::Y> *>>>
		UiConstraints &SetY(Args &&... args) {
		m_y = std::make_unique<T<UiConstraintType::Y>>(std::forward<Args>(args)...);
		return *this;
	}

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::Width> *, UiConstraint<UiConstraintType::Width> *>>>
		UiConstraints &SetWidth(Args &&... args) {
		m_width = std::make_unique<T<UiConstraintType::Width>>(std::forward<Args>(args)...);
		return *this;
	}

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::Height> *, UiConstraint<UiConstraintType::Height> *>>>
		UiConstraints &SetHeight(Args &&... args) {
		m_height = std::make_unique<T<UiConstraintType::Height>>(std::forward<Args>(args)...);
		return *this;
	}

	float GetDepth() const { return m_depth; }
	void SetDepth(float depth) { m_depth = depth; }

private:
	std::unique_ptr<UiConstraint<UiConstraintType::X>> m_x;
	std::unique_ptr<UiConstraint<UiConstraintType::Y>> m_y;
	std::unique_ptr<UiConstraint<UiConstraintType::Width>> m_width;
	std::unique_ptr<UiConstraint<UiConstraintType::Height>> m_height;

	float m_depth = 0.0f;
};
}