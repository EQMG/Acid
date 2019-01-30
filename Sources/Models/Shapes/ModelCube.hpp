#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelCube :
		public Model
	{
	public:
		static std::shared_ptr<ModelCube> Create(const Metadata &metadata);

		static std::shared_ptr<ModelCube> Create(const float &width, const float &height, const float &depth);

		explicit ModelCube(const float &width = 1.0f, const float &height = 1.0f, const float &depth = 1.0f);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		float m_width;
		float m_height;
		float m_depth;
	};
}
