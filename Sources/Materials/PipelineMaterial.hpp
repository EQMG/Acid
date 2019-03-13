#pragma once

#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderStage.hpp"

namespace acid
{
/// <summary>
/// Class
/// that
/// represents
/// a
/// material
/// pipeline.
/// </summary>
class ACID_EXPORT PipelineMaterial : public Resource
{
  public:
	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// material
	/// pipeline
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// material
	/// pipeline.
	/// </summary>
	/// <param
	/// name="pipelineStage">
	/// Stage
	/// the
	/// pipeline
	/// will
	/// be
	/// executed
	/// on.
	/// </param>
	/// <param
	/// name="pipelineCreate">
	/// Information
	/// used
	/// to
	/// define
	/// pipeline
	/// properties.
	/// </param>
	static std::shared_ptr<PipelineMaterial> Create(const Pipeline::Stage& pipelineStage, const PipelineGraphicsCreate& pipelineCreate);

	/// <summary>
	/// Creates
	/// a
	/// new
	/// material
	/// pipeline.
	/// </summary>
	/// <param
	/// name="pipelineStage">
	/// Stage
	/// the
	/// pipeline
	/// will
	/// be
	/// executed
	/// on.
	/// </param>
	/// <param
	/// name="pipelineCreate">
	/// Information
	/// used
	/// to
	/// define
	/// pipeline
	/// properties.
	/// </param>
	PipelineMaterial(const Pipeline::Stage& pipelineStage, const PipelineGraphicsCreate& pipelineCreate);

	/// <summary>
	/// Binds
	/// this
	/// pipeline
	/// to
	/// the
	/// current
	/// renderpass.
	/// </summary>
	/// <param
	/// name="commandBuffer">
	/// The
	/// command
	/// buffer
	/// to
	/// write
	/// to.
	/// </param>
	bool BindPipeline(const CommandBuffer& commandBuffer);

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	const Pipeline::Stage& GetStage() const
	{
		return m_pipelineStage;
	}

	const PipelineGraphicsCreate& GetPipelineCreate() const
	{
		return m_pipelineCreate;
	}

	const PipelineGraphics* GetPipeline()
	{
		return m_pipeline.get();
	}

  private:
	Pipeline::Stage m_pipelineStage;
	PipelineGraphicsCreate m_pipelineCreate;
	const RenderStage* m_renderStage;
	std::unique_ptr<PipelineGraphics> m_pipeline;
};
}
