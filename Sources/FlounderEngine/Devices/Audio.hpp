#pragma once

#include "../Engine/Engine.hpp"
#include "../Engine/Platform.hpp"
#include "../Sounds/Sound.hpp"

namespace Flounder
{
	struct F_HIDDEN SoundSourceInfo
	{
		unsigned int m_size;
		unsigned char *m_data;
		short m_formatTag;
		short m_channels;
		int m_samplesPerSec;
		int m_averageBytesPerSec;
		short m_blockAlign;
		short m_bitsPerSample;
	};

	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class F_EXPORT Audio :
		public IModule
	{
	private:
		ALCdevice *m_alDevice;
		ALCcontext *m_alContext;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Audio *Get()
		{
			return reinterpret_cast<Audio *>(Engine::Get()->GetModule("audio"));
		}

		/// <summary>
		/// Creates a new audio module.
		/// </summary>
		Audio();

		/// <summary>
		/// Deconstructor for the audio module.
		/// </summary>
		~Audio();

		void Update() override;

		static ALuint LoadFileWav(const std::string &filename);

		static ALuint LoadFileOgg(const std::string &filename);

	private:
		static void LogOpenAlSound(const std::string &path, const SoundSourceInfo &sourceInfo);
	};
}
