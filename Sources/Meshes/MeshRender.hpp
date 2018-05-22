#pragma once

#include "Materials/IMaterial.hpp"
#include "Mesh.hpp"

namespace fl
{
	class FL_EXPORT MeshRender :
		public Component
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformObject;
	public:
		MeshRender();

		~MeshRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const CommandBuffer &commandBuffer, UniformHandler *uniformScene);

		std::string GetName() const override { return "MeshRender"; };

		UniformHandler *GetUniformObject() const { return m_uniformObject; }
	};
}
