#pragma once

#include "Mesh.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"

namespace acid
{
class ACID_EXPORT MeshRender : public Component
{
  public:
	void Start() override;

	void Update() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	bool CmdRender(const CommandBuffer& commandBuffer, UniformHandler& uniformScene, const Pipeline::Stage& pipelineStage);

	bool operator<(const MeshRender& other) const;

  private:
	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;
};
}
