#pragma once

#include "Scenes/System.hpp"

// TODO: Only exists until all components are moved into the new entity component system.

namespace acid {
class ACID_EXPORT ShimSystem : public System {
public:
	ShimSystem();
	
	void Update(float delta) override;
};
}
