#include "UiConstraints.hpp"

#include "PixelConstraint.hpp"
#include "RelativeConstraint.hpp"

namespace acid {
UiConstraints::UiConstraints() {
	SetX<PixelConstraint>(0);
	SetY<PixelConstraint>(0);
	SetWidth<RelativeConstraint>(1.0f);
	SetHeight<RelativeConstraint>(1.0f);
}

bool UiConstraints::Update(const UiConstraints *parent) {
	bool dirty = false;
	dirty |= x->Update(this, parent);
	dirty |= y->Update(this, parent);
	dirty |= width->Update(this, parent);
	dirty |= height->Update(this, parent);
	return dirty;
}
}