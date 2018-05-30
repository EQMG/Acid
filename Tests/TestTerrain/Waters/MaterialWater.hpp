#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>

using namespace fl;

namespace test
{
	/// <summary>
	/// Class that represents a water material shader.
	/// </summary>
	class MaterialWater :
		public IMaterial
	{
	private:
		Colour m_colour;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialWater();

		~MaterialWater();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler *uniformObject) override;

		void PushDescriptors(DescriptorsHandler *descriptorSet) override;

		std::string GetName() const override { return "MaterialWater"; };

		Colour GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
