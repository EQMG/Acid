#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelSphere :
		public Model
	{
	public:
		static std::shared_ptr<ModelSphere> Create(const Metadata &metadata);

		static std::shared_ptr<ModelSphere> Create(const float &radius, const uint32_t &latitudeBands, const uint32_t &longitudeBands);

		explicit ModelSphere(const float &radius = 1.0f, const uint32_t &latitudeBands = 20, const uint32_t &longitudeBands = 20);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_radius;
		uint32_t m_latitudeBands;
		uint32_t m_longitudeBands;
	};
}
