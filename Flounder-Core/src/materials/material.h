#pragma once

#include <string>

#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"
#include "../maths/colour.h"
#include "../textures/texture.h"

namespace flounder
{
	/// <summary>
	/// Class that represents a MTL material.
	/// </summary>
	class material
	{
	private:
		std::string m_name;
		std::string m_file;

		float m_specularCoefficient;
		colour *m_ambient;
		colour *m_diffuse;
		colour *m_specular;

		texture *m_albedo;
		texture *m_normals;
	public:
		/// <summary>
		/// Creates a new material.
		/// </summary>
		/// <param name="name"> The file name. </param>
		material(const std::string &file);

		/// <summary>
		/// Deconstructor for the material.
		/// </summary>
		~material();

		std::string getName() const { return m_name; }

		std::string getFile() const { return m_file; }

		float getSpecularCoefficient() const { return m_specularCoefficient; }

		colour *getAmbient() const { return m_ambient; }

		colour *getDiffuse() const { return m_diffuse; }

		colour *getSpecular() const { return m_specular; }

		texture *getAlbedo() const { return m_albedo; }

		texture *getNormals() const { return m_normals; }
	private:
		/// <summary>
		/// Loads the material object from a MTL file.
		/// </summary>
		void loadFromFile();
	};
}
