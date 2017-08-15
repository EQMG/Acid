#include "texture.h"

namespace flounder
{
	texture::texture(std::string file, const bool &hasAlpha, const bool &clampEdges, const uint32_t &mipLevels, const bool &anisotropic, const bool &nearest, const uint32_t &numberOfRows)
	{
		m_file = file;
		m_cubemapCount = 0;
		m_cubemap = nullptr;

		m_hasAlpha = hasAlpha;
		m_clampEdges = clampEdges;
		m_mipLevels = mipLevels;
		m_anisotropic = anisotropic;
		m_nearest = nearest;
		m_numberOfRows = numberOfRows;

		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;
		m_format = VK_FORMAT_UNDEFINED;
		m_imageType = VK_IMAGE_TYPE_2D;
		m_components = 0;
		m_width = 0;
		m_height = 0;
		m_depth = 1;

		loadFromTexture();
	}

	texture::texture(const int n_args, ...)
	{
		m_file = "";
		m_cubemapCount = n_args;
		m_cubemap = new std::string[n_args];

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_cubemap[i] = va_arg(ap, const char*);
		}

		va_end(ap);

		m_hasAlpha = false;
		m_clampEdges = false;
		m_mipLevels = 1;
		m_anisotropic = true;
		m_nearest = false;
		m_numberOfRows = 1;

		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;
		m_format = VK_FORMAT_UNDEFINED;
		m_imageType = VK_IMAGE_TYPE_2D;
		m_components = 0;
		m_width = 0;
		m_height = 0;
		m_depth = 1;

		loadFromCubemap();
	}

	texture::~texture()
	{
	//	vkDestroyImageView(display::get()->getVkDevice(), m_imageView, nullptr);
	//	vkFreeMemory(display::get()->getVkDevice(), m_imageMemory, nullptr);
	//	vkDestroyImage(display::get()->getVkDevice(), m_image, nullptr);
	}

	void texture::loadFromTexture()
	{
		stbi_uc *stbidata = loadPixels(m_file, &m_width, &m_height, &m_components);

		float *pixels = new float[m_width * m_height * m_components];
		
		for (int i = 0; i < m_width * m_height * m_components; i++)
		{
			float f = static_cast<float>(stbidata[i]) / static_cast<float>(static_cast<unsigned char>(-1));
			pixels[i] = f;
		}

		stbi_image_free(stbidata);

		switch (m_components) {
		case 1:
			m_format = VK_FORMAT_R32_SFLOAT;
			break;
		case 2:
			m_format = VK_FORMAT_R32G32_SFLOAT;
			break;
		case 3:
			m_format = VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case 4:
			m_format = VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		default:
			assert(0 && "Vulkan texture components not between 1-4.");
		}

		delete[] pixels;
	}

	void texture::loadFromCubemap()
	{

	}

	stbi_uc *texture::loadPixels(const std::string &filepath, int *width, int *height, int *channels)
	{
		stbi_uc* pixels = nullptr;
#ifdef FLOUNDER_PLATFORM_ANDROID
		AAsset* asset = AAssetManager_open((display::get()->getAssetManager(), filepath.c_str(), AASSET_MODE_STREAMING);

		if (!asset) 
		{
			std::cout << "Unable to load android asset: " << m_file << std::endl;
			freePixels(layersPixels);
			return nullptr;
		}

		uint32_t size = AAsset_getLength(asset);

		if (size <= 0) 
		{
			std::cout << "Android asset is empty: " << m_file << std::endl;
			freePixels(layersPixels);
			return nullptr;
		}

		unsigned char* data = new unsigned char[size];

		AAsset_read(asset, reinterpret_cast<char*>(data), size);
		AAsset_close(asset);

		pixels = stbi_load_from_memory(data, size, width, height, channels, STBI_rgb_alpha);

		delete[] data;
#else
		if (stbi_info(filepath.c_str(), &m_width, &m_height, &m_components) == 0)
		{
			assert(0 && "Vulkan invalid texture file format.");
		}

		pixels = stbi_load(filepath.c_str(), width, height, channels, STBI_rgb_alpha);
#endif

		if (pixels == nullptr)
		{
			std::cout << "Unable to load texture: " << m_file << std::endl;
		}

		return pixels;
	}
}
