#pragma once

#include "Maths/Colour.hpp"
#include "Materials/IMaterial.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a water material shader.
	/// </summary>
	class FL_EXPORT MaterialWater :
		public IMaterial
	{
	private:
		Colour *m_colour;

		PipelineMaterial *m_material;
	public:
		MaterialWater();

		~MaterialWater();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler *uniformObject) override;

		void PushDescriptors(DescriptorsHandler *descriptorSet) override;

		std::string GetName() const override { return "MaterialWater"; };

		Colour *GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) const { *m_colour = colour; }

		PipelineMaterial *GetMaterial() const override { return m_material; }
	};
}
