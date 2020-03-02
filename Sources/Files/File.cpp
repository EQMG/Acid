#include "File.hpp"

#include "Engine/Engine.hpp"
#include "Json/Json.hpp"
#include "Xml/Xml.hpp"
#include "Files.hpp"

namespace acid {
File::File(std::unique_ptr<Node> &&node) :
	node(std::move(node)) {
}

File::File(const std::filesystem::path &filename) :
	filename(filename) {
	// TODO: Node factory.
	if (filename.extension() == ".json") {
		node = std::make_unique<Json>();
	} else if (filename.extension() == ".xml") {
		node = std::make_unique<Xml>("root");
	}
}

File::File(std::filesystem::path filename, std::unique_ptr<Node> &&node) :
	node(std::move(node)),
	filename(std::move(filename)) {
}

void File::Load(const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	if (Files::ExistsInPath(filename)) {
		IFStream inStream(filename);
		node->ParseStream(inStream);
	} else if (std::filesystem::exists(filename)) {
		std::ifstream inStream(filename);
		node->ParseStream(inStream);
		inStream.close();
	}

#if defined(ACID_DEBUG)
	Log::Out("File ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Load() {
	Load(filename);
}

void File::Write(const std::filesystem::path &filename, Node::Format format) const {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	/*if (Files::ExistsInPath(filename)) {
		OFStream os(filename);
		node->WriteStream(os, format);
	} else {*/ // if (std::filesystem::exists(filename))
		if (auto parentPath = filename.parent_path(); !parentPath.empty()) {
			std::filesystem::create_directories(parentPath);
		}

		std::ofstream os(filename);
		node->WriteStream(os, format);
		os.close();
	//}

#if defined(ACID_DEBUG)
	Log::Out("File ", filename, " saved in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Write(Node::Format format) const {
	Write(filename, format);
}

void File::Clear() {
	node->Clear();
}
}
