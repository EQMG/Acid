#pragma once

#include "../Engine/Engine.hpp"
#include "../Platforms/Platform.hpp"
#include "../Sounds/Sound.hpp"

namespace Flounder
{
	struct SoundSourceInfo
	{
		unsigned int size;
		unsigned char *data;
		short formatTag;
		short channels;
		int samplesPerSec;
		int averageBytesPerSec;
		short blockAlign;
		short bitsPerSample;
	};

	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class Audio :
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
			return dynamic_cast<Audio *>(Engine::Get()->GetModule("audio"));
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
