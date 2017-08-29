#pragma once

#include "../entity.hpp"
#include "../icomponent.hpp"

namespace flounder
{
	class componentalpha :
		public icomponent
	{
	private:
		float m_alpha;
	public:
		componentalpha(const float &alpha);

		~componentalpha();

		void update() override;

		void render(entityrender *render) override;

		float getAlpha() const { return m_alpha; }

		void setAlpha(const float &alpha) { m_alpha = alpha; }
	};
}
