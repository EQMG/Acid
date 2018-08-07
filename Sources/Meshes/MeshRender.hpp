#pragma once

#include "Materials/IMaterial.hpp"
#include "Mesh.hpp"

namespace acid
{
	class ACID_EXPORT MeshRender :
		public IComponent
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;
	public:
		MeshRender();

		~MeshRender();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene);

		UniformHandler GetUniformObject() const { return m_uniformObject; }
	};
}
