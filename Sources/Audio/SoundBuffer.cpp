#include "SoundBuffer.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"

namespace acid {
std::shared_ptr<SoundBuffer> SoundBuffer::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<SoundBuffer>(node))
		return resource;

	auto result = std::make_shared<SoundBuffer>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<SoundBuffer> SoundBuffer::Create(const std::filesystem::path &filename) {
	SoundBuffer temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

SoundBuffer::SoundBuffer(std::filesystem::path filename, bool load) :
	filename(std::move(filename)) {
	if (load)
		SoundBuffer::Load();
}

SoundBuffer::~SoundBuffer() {
	alDeleteBuffers(1, &buffer);
}

void SoundBuffer::SetBuffer(uint32_t buffer) {
	if (this->buffer)
		alDeleteBuffers(1, &this->buffer);
	this->buffer = buffer;
}

const Node &operator>>(const Node &node, SoundBuffer &soundBuffer) {
	node["filename"].Get(soundBuffer.filename);
	return node;
}

Node &operator<<(Node &node, const SoundBuffer &soundBuffer) {
	node["filename"].Set(soundBuffer.filename);
	return node;
}

void SoundBuffer::Load() {
	if (filename.empty())
		return;

	Registry()[filename.extension().string()].first(this, filename);
}
}
