#pragma once

#include <string>
#include <vector>
#include <stdarg.h>

#include "../maths/colour.h"

namespace flounder {
	/// <summary>
	/// A builder used to set texture parameters for loading.
	/// </summary>
	class texturebuilder
	{
	public:
		std::string m_file;
		std::string *m_cubemap;
		int m_cubemapCount;
		colour *m_border;
		bool m_clampToBorder;
		bool m_clampEdges;
		bool m_mipmap;
		bool m_anisotropic;
		bool m_nearest;
		int m_numberOfRows;

		/// <summary>
		/// Creates a new texture builder.
		/// </summary>
		texturebuilder();

		/// <summary>
		/// Deconstructor for the texture builder.
		/// </summary>
		~texturebuilder();

		/// <summary>
		/// Sets the textures source file.
		/// </summary>
		/// <param name="file"> The source file. </param>
		/// <returns> This. </returns>
		texturebuilder *setFile(const std::string &file);

		/// <summary>
		/// Sets the cubemap source files.
		/// </summary>
		/// <param name="n_args"> The number of cubemap files being loaded. </param>
		/// <param name="..."> The source cubemap files. </param>
		/// <returns> This. </returns>
		texturebuilder *setCubemap(const int n_args, ...);

		/// <summary>
		/// Clamps the texture to a coloured border.
		/// </summary>
		/// <param name="border"> The coloured border. </param>
		/// <returns> This. </returns>
		texturebuilder *clampToBorder(colour *border);

		/// <summary>
		/// Clamps the texture to the edges.
		/// </summary>
		/// <returns> This. </returns>
		texturebuilder *clampEdges();

		/// <summary>
		/// Selects nearest filtering.
		/// </summary>
		/// <returns> This. </returns>
		texturebuilder *nearestFiltering();

		/// <summary>
		/// Disables mipmapping.
		/// </summary>
		/// <returns> This. </returns>
		texturebuilder *noMipmap();

		/// <summary>
		/// Disables anisotropic filtering.
		/// </summary>
		/// <returns> This. </returns>
		texturebuilder *noFiltering();

		/// <summary>
		/// Sets the starting number of texture rows (default = 1).
		/// </summary>
		/// <param name="numberOfRows"> The new number of rows.
		/// </param>
		/// <returns> This. </returns>
		texturebuilder *setNumberOfRows(const int numberOfRows);
	};
}
