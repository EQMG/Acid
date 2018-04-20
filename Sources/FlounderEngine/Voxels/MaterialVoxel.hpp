#pragma once

#include "Materials/IMaterial.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a voxel material shader.
	/// </summary>
	class F_EXPORT MaterialVoxel :
		public IMaterial
	{
	private:
		PipelineMaterial *m_material;
	public:
		MaterialVoxel();

		~MaterialVoxel();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler *uniformObject) override;

		void PushDescriptors(DescriptorsHandler *descriptorSet) override;

		std::string GetName() const override { return "MaterialVoxel"; };

		PipelineMaterial *GetMaterial() const override { return m_material; }
	};
}
