#pragma once

#include "Utils/NonCopyable.hpp"
#include "Utils/TypeInfo.hpp"
#include "SceneExport.hpp"

namespace acid {
class ACID_SCENE_EXPORT System : NonCopyable {
public:
	virtual ~System() = default;

	virtual void Update() = 0;

	bool IsEnabled() const { return enabled; }
	void SetEnabled(bool enable) { this->enabled = enable; }

private:
	bool enabled = true;
};

template class ACID_SCENE_EXPORT TypeInfo<System>;
}
