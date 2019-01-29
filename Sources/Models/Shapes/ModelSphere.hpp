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

		ModelSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);

	//	void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		uint32_t m_latitudeBands;
		uint32_t m_longitudeBands;
		float m_radius;
	};
}
