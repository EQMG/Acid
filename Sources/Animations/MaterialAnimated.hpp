#pragma once

#include "Maths/Colour.hpp"
#include "Models/Model.hpp"
#include "Textures/Texture.hpp"
#include "Materials/IMaterial.hpp"

namespace fl
{
	class FL_EXPORT MaterialAnimated :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialAnimated();

		~MaterialAnimated();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::string GetName() const override { return "MaterialAnimated"; };

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
