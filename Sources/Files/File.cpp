#include "File.hpp"

#include "Engine/Engine.hpp"
#include "Json/Json.hpp"
#include "Xml/Xml.hpp"
#include "Files.hpp"

namespace acid {
File::File(std::unique_ptr<NodeFormat> &&type, const Node &node) :
	node(node),
	type(std::move(type)) {
}

File::File(std::unique_ptr<NodeFormat> &&type, Node &&node) :
	node(std::move(node)),
	type(std::move(type)) {
}

File::File(std::filesystem::path filename, std::unique_ptr<NodeFormat>&& type, const Node &node) :
	node(node),
	type(std::move(type)),
	filename(std::move(filename)) {
}

File::File(std::filesystem::path filename, std::unique_ptr<NodeFormat>&& type, Node &&node) :
	node(std::move(node)),
	type(std::move(type)),
	filename(std::move(filename)) {
}

void File::Load(const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	if (Files::ExistsInPath(filename)) {
		IFStream inStream(filename);
		node = type->ParseStream(inStream);
	} else if (std::filesystem::exists(filename)) {
		std::ifstream inStream(filename);
		node = type->ParseStream(inStream);
		inStream.close();
	}

#ifdef ACID_DEBUG
	Log::Out("File ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Load() {
	Load(filename);
}

void File::Write(const std::filesystem::path &filename, NodeFormat::Format format) const {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	/*if (Files::ExistsInPath(filename)) {
		OFStream os(filename);
		node->WriteStream(os, *formatter);
	} else {*/ // if (std::filesystem::exists(filename))
		if (auto parentPath = filename.parent_path(); !parentPath.empty())
			std::filesystem::create_directories(parentPath);

		std::ofstream os(filename);
		type->WriteStream(node, os, format);
		os.close();
	//}

#ifdef ACID_DEBUG
	Log::Out("File ", filename, " saved in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void File::Write(NodeFormat::Format format) const {
	Write(filename, format);
}

void File::Clear() {
	node.Clear();
}
}
