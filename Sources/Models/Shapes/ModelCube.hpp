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
		static std::shared_ptr<ModelCube> Create(const float &width, const float &height, const float &depth);

		static std::shared_ptr<ModelCube> Create(const std::string &data);

		ModelCube(const float &width, const float &height, const float &depth);

		void Encode(Metadata &metadata) const override;
	private:
		static std::string ToName(const float &width, const float &height, const float &depth);

		float m_width;
		float m_height;
		float m_depth;
	};
}
