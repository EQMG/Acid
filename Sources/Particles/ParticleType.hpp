#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Textures/Texture.hpp"
#include "Resources/Resources.hpp"

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
		Colour m_colourOffset;
		float m_lifeLength;
		float m_scale;
	public:
		static std::shared_ptr<ParticleType> Resource(const std::shared_ptr<Texture> &texture, const Colour &colourOffset, const float &lifeLength, const float &scale)
		{
			auto resource = Resources::Get()->Get(ToFilename(texture, colourOffset, lifeLength, scale));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ParticleType>(resource);
			}

			auto result = std::make_shared<ParticleType>(texture, colourOffset, lifeLength, scale);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ParticleType> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			auto texture = Texture::Resource(split[1].c_str());
			Colour colourOffset = Colour(split[2].c_str());
			float lifeLength = static_cast<float>(atof(split[3].c_str()));
			float scale = static_cast<float>(atof(split[4].c_str()));
			return Resource(texture, colourOffset, lifeLength, scale);
		}

		/// <summary>
		/// Creates a new particle type.
		/// </summary>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="colourOffset"> The particles texture colour offset. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		ParticleType(std::shared_ptr<Texture> texture, const Colour &colourOffset, const float &lifeLength, const float &scale);

		/// <summary>
		/// Deconstructor for the particle type.
		/// </summary>
		~ParticleType();

		std::string GetName() override { return m_filename; }

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		void SetTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

		Colour GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) { m_colourOffset = colourOffset; }

		float GetLifeLength() const { return m_lifeLength; }

		void SetLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		float GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }
	private:
		static std::string ToFilename(const std::shared_ptr<Texture> &texture, const Colour &colourOffset, const float &lifeLength, const float &scale);
	};
}
