#include "File.hpp"

#include "Engine/Engine.hpp"
#include "Files.hpp"
#include "FileSystem.hpp"

namespace acid
{
	File::File(const std::string &filename, Metadata *metadata) :
		m_filename(filename),
		m_metadata(metadata)
	{
	}

	void File::Read()
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto fileLoaded = Files::Read(m_filename);

		if (!fileLoaded)
		{
			Log::Error("File could not be loaded: '%s'\n", m_filename.c_str());
			return;
		}

		m_metadata->Load(*fileLoaded);

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("File '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	void File::Write()
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		std::string data = m_metadata->Write();

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("File '%s' saved in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	void File::Clear()
	{
		m_metadata->ClearChildren();

		Verify();
		FileSystem::ClearFile(m_filename);
	}

	void File::Verify()
	{
		if (!FileSystem::Exists(m_filename))
		{
			FileSystem::Create(m_filename);
		}
	}
}
