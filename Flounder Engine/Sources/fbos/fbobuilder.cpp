#include "fbobuilder.h"

namespace flounder {
	fbobuilder::fbobuilder(const int width, const int height)
	{
		m_depthBufferType = NONE;
		m_useColourBuffer = true;
		m_linearFiltering = true;
		m_wrapTextures = true;
		m_clampEdge = true;
		m_alphaChannel = false;
		m_antialiased = false;
		m_samples = 0;
		m_width = width;
		m_height = height;
		m_attachments = 1;
		m_fitToScreen = false;
		m_sizeScalar = 1.0f;
	}

	fbobuilder::fbobuilder()
		: fbobuilder(display::get()->getWidth(), display::get()->getHeight())
	{
		fitToScreen(true);
	}

	fbobuilder::~fbobuilder()
	{
	}

	fbobuilder *fbobuilder::depthBuffer(const depthbuffer type)
	{
		m_depthBufferType = type;
		return this;
	}

	fbobuilder *fbobuilder::noColourBuffer()
	{
		m_useColourBuffer = false;
		return this;
	}

	fbobuilder *fbobuilder::nearestFiltering()
	{
		m_linearFiltering = false;
		return this;
	}

	fbobuilder *fbobuilder::disableTextureWrap()
	{
		m_wrapTextures = false;
		return this;
	}

	fbobuilder *fbobuilder::repeatTexture()
	{
		m_clampEdge = false;
		return this;
	}

	fbobuilder *fbobuilder::withAlphaChannel(const bool alpha)
	{
		m_alphaChannel = alpha;
		return this;
	}

	fbobuilder *fbobuilder::antialias(const int samples)
	{
		m_antialiased = true;
		m_samples = samples;
		return this;
	}

	fbobuilder *fbobuilder::attachments(const int attachments)
	{
		m_attachments = attachments;
		return this;
	}

	fbobuilder *fbobuilder::fitToScreen(const float sizeScalar)
	{
		m_fitToScreen = true;
		m_sizeScalar = sizeScalar;
		return this;
	}

}
