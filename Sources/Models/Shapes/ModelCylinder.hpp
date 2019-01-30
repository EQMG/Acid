#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelCylinder :
		public Model
	{
	public:
		static std::shared_ptr<ModelCylinder> Create(const Metadata &metadata);

		static std::shared_ptr<ModelCylinder> Create(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);

		explicit ModelCylinder(const float &radiusBase = 1.0f, const float &radiusTop = 1.0f, const float &height = 2.0f, const uint32_t &slices = 20, const uint32_t &stacks = 2);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_radiusBase;
		float m_radiusTop;
		float m_height;
		uint32_t m_slices;
		uint32_t m_stacks;
	};
}
