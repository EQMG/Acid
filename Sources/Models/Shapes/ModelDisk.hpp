#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelDisk :
		public Model
	{
	public:
		static std::shared_ptr<ModelDisk> Create(const Metadata &metadata);

		static std::shared_ptr<ModelDisk> Create(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);

		explicit ModelDisk(const float &innerRadius = 0.5f, const float &outerRadius = 1.0f, const uint32_t &slices = 20, const uint32_t &loops = 4);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_innerRadius;
		float m_outerRadius;
		uint32_t m_slices;
		uint32_t m_loops;
	};
}
