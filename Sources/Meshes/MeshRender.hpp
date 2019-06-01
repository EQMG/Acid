#pragma once

#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Mesh.hpp"

namespace acid
{
class ACID_EXPORT MeshRender :
	public Component
{
public:
	void Start() override;

	void Update() override;

	bool CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage);

	bool operator<(const MeshRender &other) const;

	friend const Metadata &operator>>(const Metadata &metadata, MeshRender &meshRender);

	friend Metadata &operator<<(Metadata &metadata, const MeshRender &meshRender);

private:
	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;
};
}
