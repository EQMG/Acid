#include "File.hpp"
#include <utility>

#include "Engine/Engine.hpp"
#include "Json/Json.hpp"
#include "Xml/Xml.hpp"
#include "Files.hpp"

namespace acid {
File::File(std::unique_ptr<Node> &&node) :
	m_node(std::move(node)) {
}

File::File(const std::filesystem::path &filename) :
	m_filename(filename) {
	// TODO: Node factory.
	if (filename.extension() == ".json") {
		m_node = std::make_unique<Json>();
	} else if (filename.extension() == ".xml") {
		m_node = std::make_unique<Xml>("root");
	}
}

File::File(std::filesystem::path filename, std::unique_ptr<Node> &&node) :
	m_node(std::move(node)),
	m_filename(std::move(filename)) {
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

void File::Load() {
	Load(m_filename);
}

void File::Write(const std::filesystem::path &filename, Node::Format format) const {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	/*if (Files::ExistsInPath(filename)) {
		OFStream os(filename);
		m_node->WriteStream(os, format);
	} else {*/ // if (std::filesystem::exists(filename))
		if (auto parentPath = filename.parent_path(); !parentPath.empty()) {
			std::filesystem::create_directories(parentPath);
		}

		std::ofstream os(filename);
		m_node->WriteStream(os, format);
		os.close();
	//}

#if defined(ACID_DEBUG)
	Log::Out("File ", filename, " saved in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Write(Node::Format format) const {
	Write(m_filename, format);
}

void File::Clear() {
	m_node->Clear();
}
}
