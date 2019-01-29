#pragma once

#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelRectangle :
		public Model
	{
	public:
		static std::shared_ptr<ModelRectangle> Create(const float &min, const float &max);

		ModelRectangle(const float &width, const float &height);

	//	void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_min;
		float m_max;
	};
}
