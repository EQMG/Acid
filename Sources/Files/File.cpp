#include "File.hpp"

#include "Engine/Engine.hpp"
#include "Files.hpp"

namespace acid {
File::File(std::unique_ptr<Node> &&node) :
	m_node(std::move(node)) {
}

void File::Load(const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	if (Files::ExistsInPath(filename)) {
		IFStream inStream(filename);
		m_node->LoadStream(inStream);
	} else if (std::filesystem::exists(filename)) {
		std::ifstream inStream(filename);
		m_node->LoadStream(inStream);
		inStream.close();
	}

#if defined(ACID_DEBUG)
	Log::Out("File ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Write(const std::filesystem::path &filename, Node::Format format) const {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	if (Files::ExistsInPath(filename)) {
		OFStream os(filename);
		m_node->WriteStream(os, format);
	} else { // if (std::filesystem::exists(filename))
		if (auto parentPath = filename.parent_path(); !parentPath.empty()) {
			std::filesystem::create_directories(parentPath);
		}

		std::ofstream os(filename);
		m_node->WriteStream(os, format);
		os.close();
	}

#if defined(ACID_DEBUG)
	Log::Out("File ", filename, " saved in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Clear() {
	m_node->Clear();
}
}
