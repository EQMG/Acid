#pragma once

#include <memory>

#include "Utils/NonCopyable.hpp"
#include "UiConstraint.hpp"

namespace acid {
class ACID_EXPORT UiConstraints final : NonCopyable {
public:
	UiConstraints();

	bool Update(const UiConstraints *parent);

	UiConstraint<UiConstraintType::X> *GetX() const { return x.get(); }
	UiConstraint<UiConstraintType::Y> *GetY() const { return y.get(); }
	UiConstraint<UiConstraintType::Width> *GetWidth() const { return width.get(); }
	UiConstraint<UiConstraintType::Height> *GetHeight() const { return height.get(); }

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::X> *, UiConstraint<UiConstraintType::X> *>>>
		UiConstraints &SetX(Args &&... args) {
		x = std::make_unique<T<UiConstraintType::X>>(std::forward<Args>(args)...);
		return *this;
	}

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::Y> *, UiConstraint<UiConstraintType::Y> *>>>
		UiConstraints &SetY(Args &&... args) {
		y = std::make_unique<T<UiConstraintType::Y>>(std::forward<Args>(args)...);
		return *this;
	}

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::Width> *, UiConstraint<UiConstraintType::Width> *>>>
		UiConstraints &SetWidth(Args &&... args) {
		width = std::make_unique<T<UiConstraintType::Width>>(std::forward<Args>(args)...);
		return *this;
	}

	template<template<UiConstraintType, typename = void> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<UiConstraintType::Height> *, UiConstraint<UiConstraintType::Height> *>>>
		UiConstraints &SetHeight(Args &&... args) {
		height = std::make_unique<T<UiConstraintType::Height>>(std::forward<Args>(args)...);
		return *this;
	}

	float GetDepth() const { return depth; }
	void SetDepth(float depth) { this->depth = depth; }

private:
	std::unique_ptr<UiConstraint<UiConstraintType::X>> x;
	std::unique_ptr<UiConstraint<UiConstraintType::Y>> y;
	std::unique_ptr<UiConstraint<UiConstraintType::Width>> width;
	std::unique_ptr<UiConstraint<UiConstraintType::Height>> height;

	float depth = 0.0f;
};
}