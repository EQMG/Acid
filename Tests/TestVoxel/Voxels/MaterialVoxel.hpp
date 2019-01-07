#pragma once

#include <Materials/Material.hpp>

using namespace acid;

namespace test
{
	/// <summary>
	/// Class that represents a voxel material shader.
	/// </summary>
	class MaterialVoxel :
		public Material
	{
	public:
		MaterialVoxel();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;
	};
}
