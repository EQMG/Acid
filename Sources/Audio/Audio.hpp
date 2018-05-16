#pragma once

#include "Engine/Engine.hpp"

typedef struct ALCdevice_struct ALCdevice;

typedef struct ALCcontext_struct ALCcontext;

namespace fl
{
	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class FL_EXPORT Audio :
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

		FL_HIDDEN static std::string StringifyResultAl(const int &result);

		FL_HIDDEN static void ErrorAl(const int &result);

		FL_HIDDEN ALCdevice *GetAlcDevice() const { return m_alDevice; }

		FL_HIDDEN ALCcontext *GetAlcContext() const { return m_alContext; }
	};
}
