#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Resources/Resources.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	/// <summary>
	/// A definition for what a particle should act and look like.
	/// </summary>
	class ACID_EXPORT ParticleType :
		public IResource
	{
	private:
		std::string m_filename;
		std::shared_ptr<Texture> m_texture;
		uint32_t m_numberOfRows;
		Colour m_colourOffset;
		float m_lifeLength;
		float m_scale;
	public:
		static std::shared_ptr<ParticleType> Resource(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale)
		{
			auto resource = Resources::Get()->Get(ToFilename(texture, numberOfRows, colourOffset, lifeLength, scale));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ParticleType>(resource);
			}

			auto result = std::make_shared<ParticleType>(texture, numberOfRows, colourOffset, lifeLength, scale);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ParticleType> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			auto texture = Texture::Resource(split[1].c_str());
			uint32_t numberOfRows = static_cast<uint32_t>(atof(split[2].c_str()));
			Colour colourOffset = Colour(split[3].c_str());
			float lifeLength = static_cast<float>(atof(split[4].c_str()));
			float scale = static_cast<float>(atof(split[5].c_str()));
			return Resource(texture, numberOfRows, colourOffset, lifeLength, scale);
		}

		/// <summary>
		/// Creates a new particle type.
		/// </summary>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		/// <param name="colourOffset"> The particles texture colour offset. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		ParticleType(std::shared_ptr<Texture> texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale);

		/// <summary>
		/// Deconstructor for the particle type.
		/// </summary>
		~ParticleType();

		std::string GetFilename() override { return m_filename; }

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		void SetTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

		uint32_t GetNumberOfRows() const { return m_numberOfRows; }

		void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		Colour GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) { m_colourOffset = colourOffset; }

		float GetLifeLength() const { return m_lifeLength; }

		void SetLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		float GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }
	private:
		static std::string ToFilename(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale);
	};
}
