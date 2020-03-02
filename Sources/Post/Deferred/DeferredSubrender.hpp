#pragma once

#include "Utils/Future.hpp"
#include "Lights/Fog.hpp"
#include "Maths/Vector3.hpp"
#include "Graphics/Subrender.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Images/ImageCube.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid {
class ACID_EXPORT DeferredSubrender : public Subrender {
public:
	explicit DeferredSubrender(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

	static std::unique_ptr<Image2d> ComputeBRDF(uint32_t size);
	static std::unique_ptr<ImageCube> ComputeIrradiance(const std::shared_ptr<ImageCube> &source, uint32_t size);
	static std::unique_ptr<ImageCube> ComputePrefiltered(const std::shared_ptr<ImageCube> &source, uint32_t size);

	const Fog &GetFog() const { return fog; }
	void SetFog(const Fog &fog) { this->fog = fog; }

private:
	class DeferredLight {
	public:
		Colour colour;
		Vector3f position;
		float radius = 0.0f;
	};

	DescriptorsHandler descriptorSet;
	UniformHandler uniformScene;
	StorageHandler storageLights;

	PipelineGraphics pipeline;

	Future<std::unique_ptr<Image2d>> brdf;

	std::shared_ptr<ImageCube> skybox;

	Future<std::unique_ptr<ImageCube>> irradiance;
	Future<std::unique_ptr<ImageCube>> prefiltered;

	Fog fog;
};
}
