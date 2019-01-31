#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Resources/Resource.hpp"
#include "Textures/Texture.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class Particle;

	/// <summary>
	/// A definition for what a particle should act and look like.
	/// </summary>
	class ACID_EXPORT ParticleType :
		public Resource
	{
	public:
		/// <summary>
		/// Will find an existing particle type with the same values, or create a new particle type.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<ParticleType> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing particle type with the same values, or create a new particle type.
		/// </summary>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		/// <param name="colourOffset"> The particles texture colour offset. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="stageCycles"> The amount of times stages will be shown. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		static std::shared_ptr<ParticleType> Create(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::Black, const float &lifeLength = 10.0f, const float &stageCycles = 1.0f, const float &scale = 1.0f);

		/// <summary>
		/// Creates a new particle type.
		/// </summary>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		/// <param name="colourOffset"> The particles texture colour offset. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="stageCycles"> The amount of times stages will be shown. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		explicit ParticleType(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::Black, const float &lifeLength = 10.0f, const float &stageCycles = 1.0f, const float &scale = 1.0f);

		void Update(const std::vector<Particle> &particles);

		bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		const std::shared_ptr<Texture> &GetTexture() const { return m_texture; }

		void SetTexture(const std::shared_ptr<Texture> &texture) { m_texture = texture; }

		const uint32_t &GetNumberOfRows() const { return m_numberOfRows; }

		void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		const Colour &GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) { m_colourOffset = colourOffset; }

		const float &GetLifeLength() const { return m_lifeLength; }

		void SetLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		const float &GetStageCycles() const { return m_stageCycles; }

		void SetStageCycles(const float &stageCycles) { m_stageCycles = stageCycles; }

		const float &GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }
	private:
		struct ParticleTypeData
		{
			Matrix4 modelMatrix;
			Colour colourOffset;
			Vector4 offsets;
			Vector3 blend;
			float _padding;
		};

		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<Model> m_model;
		uint32_t m_numberOfRows;
		Colour m_colourOffset;
		float m_lifeLength;
		float m_stageCycles;
		float m_scale;

		uint32_t m_maxInstances;
		uint32_t m_instances;

		DescriptorsHandler m_descriptorSet;
		StorageHandler m_storageInstances;
	};
}
