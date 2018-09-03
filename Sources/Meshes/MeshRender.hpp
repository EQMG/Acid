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

		void Decode(const Serialized &serialized) override;

		void Encode(Serialized &serialized) const override;

		void CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const GraphicsStage &graphicsStage);

		UniformHandler GetUniformObject() const { return m_uniformObject; }

		bool operator<(const MeshRender &other) const;
	};
}
