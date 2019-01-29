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
		static std::shared_ptr<ModelCylinder> Create(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);

		ModelCylinder(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);

	//	void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_radiusBase;
		float m_radiusTop;
		float m_height;
		uint32_t m_slices;
		uint32_t m_stacks;
	};
}
