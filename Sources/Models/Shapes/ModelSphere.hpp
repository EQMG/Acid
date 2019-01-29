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
		static std::shared_ptr<ModelSphere> Create(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);

		static std::shared_ptr<ModelSphere> Create(const std::string &data);

		ModelSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);

		void Encode(Metadata &metadata) const override;
	private:
		static std::string ToName(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);

		uint32_t m_latitudeBands;
		uint32_t m_longitudeBands;
		float m_radius;
	};
}
