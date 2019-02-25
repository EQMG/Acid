#include "File.hpp"

#include <utility>
#include "Engine/Engine.hpp"
#include "Files.hpp"
#include "FileSystem.hpp"

namespace acid
{
	File::File(std::string filename, Metadata *metadata) :
		m_filename(std::move(filename)),
		m_metadata(metadata)
	{
	}

	void File::Read()
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		if (Files::ExistsInPath(m_filename))
		{
			ifstream inStream(m_filename);
			m_metadata->Load(&inStream);
		}
		else if (FileSystem::Exists(m_filename))
		{
			std::ifstream inStream(m_filename);
			m_metadata->Load(&inStream);
			inStream.close();
		}

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

		if (Files::ExistsInPath(m_filename))
		{
			ofstream outStream(m_filename);
			m_metadata->Write(&outStream);
		}
		else // if (FileSystem::Exists(m_filename))
		{
			FileSystem::Create(m_filename);
			std::ofstream outStream(m_filename);
			m_metadata->Write(&outStream);
			outStream.close();
		}

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("File '%s' saved in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	void File::Clear()
	{
		m_metadata->ClearChildren();
	}
}
