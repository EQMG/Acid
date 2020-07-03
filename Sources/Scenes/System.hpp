#pragma once

#include "Utils/TypeInfo.hpp"
#include "Export.hpp"

namespace acid {
class ACID_EXPORT System {
public:
	virtual ~System() = default;

	virtual void Update() = 0;
};

template class ACID_EXPORT TypeInfo<System>;
}
