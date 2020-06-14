#include "File.hpp"

#include "Engine/Engine.hpp"
#include "Json/Json.hpp"
#include "Xml/Xml.hpp"
#include "Files.hpp"

namespace acid {
File::File(Type type, const Node &node) :
	node(node),
	type(type) {
}

File::File(Type type, Node &&node) :
	node(std::move(node)),
	type(type) {
}

File::File(std::filesystem::path filename, Type type, const Node &node) :
	node(node),
	type(type),
	filename(std::move(filename)) {
}

File::File(std::filesystem::path filename, Type type, Node &&node) :
	node(std::move(node)),
	type(type),
	filename(std::move(filename)) {
}

void File::Load(const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	if (Files::ExistsInPath(filename)) {
		IFStream inStream(filename);
		if (type == Type::Json)
			node.ParseStream<Json>(inStream);
		else if (type == Type::Xml)
			node.ParseStream<Xml>(inStream);
	} else if (std::filesystem::exists(filename)) {
		std::ifstream inStream(filename);
		if (type == Type::Json)
			node.ParseStream<Json>(inStream);
		else if (type == Type::Xml)
			node.ParseStream<Xml>(inStream);
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
		node->WriteStream(os, *formatter);
	} else {*/ // if (std::filesystem::exists(filename))
		if (auto parentPath = filename.parent_path(); !parentPath.empty())
			std::filesystem::create_directories(parentPath);

		std::ofstream os(filename);
		if (type == Type::Json)
			node.WriteStream<Json>(os, format);
		else if (type == Type::Xml)
			node.WriteStream<Xml>(os, format);
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
	node.Clear();
}
}
