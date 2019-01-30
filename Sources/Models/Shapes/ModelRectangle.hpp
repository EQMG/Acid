#pragma once

#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelRectangle :
		public Model
	{
	public:
		static std::shared_ptr<ModelRectangle> Create(const Metadata &metadata);

		static std::shared_ptr<ModelRectangle> Create(const float &min, const float &max);

		explicit ModelRectangle(const float &min = 0.0f, const float &max = 1.0f);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_min;
		float m_max;
	};
}
