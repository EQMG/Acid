#include "File.hpp"

#include "Engine/Engine.hpp"
#include "Files.hpp"

namespace acid
{
File::File(std::filesystem::path filename, std::unique_ptr<Metadata> &&metadata) :
	m_filename{std::move(filename)},
	m_metadata{std::move(metadata)}
{
}

void File::Load()
{
#if defined(ACID_VERBOSE)
	auto debugStart{Time::Now()};
#endif

	if (Files::ExistsInPath(m_filename))
	{
		IFStream inStream{m_filename};
		m_metadata->Load(&inStream);
	}
	else if (std::filesystem::exists(m_filename))
	{
		std::ifstream inStream{m_filename};
		m_metadata->Load(&inStream);
		inStream.close();
	}

#if defined(ACID_VERBOSE)
	auto debugEnd{Time::Now()};
	Log::Out("File '%ls' loaded in %.3fms\n", m_filename, (debugEnd - debugStart).AsMilliseconds<float>());
#endif
}

void File::Write() const
{
#if defined(ACID_VERBOSE)
	auto debugStart{Time::Now()};
#endif

	if (Files::ExistsInPath(m_filename))
	{
		OFStream os{m_filename};
		m_metadata->Write(&os);
	}
	else // if (std::filesystem::exists(m_filename))
	{
		if (auto parentPath{m_filename.parent_path()}; !parentPath.empty())
		{
			std::filesystem::create_directory(parentPath);
		}

		std::ofstream os{m_filename};
		m_metadata->Write(&os);
		os.close();
	}

#if defined(ACID_VERBOSE)
	auto debugEnd{Time::Now()};
	Log::Out("File '%ls' saved in %.3fms\n", m_filename, (debugEnd - debugStart).AsMilliseconds<float>());
#endif
}

void File::Clear()
{
	m_metadata->ClearChildren();
}
}
