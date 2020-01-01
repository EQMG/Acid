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
	m_filename(std::move(filename)) {
	if (load) {
		Load();
	}
}

SoundBuffer::~SoundBuffer() {
	alDeleteBuffers(1, &m_buffer);
}

void SoundBuffer::SetBuffer(uint32_t buffer) {
	if (buffer)
		alDeleteBuffers(1, &m_buffer);
	m_buffer = buffer;
}

const Node &operator>>(const Node &node, SoundBuffer &soundBuffer) {
	node["filename"].Get(soundBuffer.m_filename);
	return node;
}

Node &operator<<(Node &node, const SoundBuffer &soundBuffer) {
	node["filename"].Set(soundBuffer.m_filename);
	return node;
}

void SoundBuffer::Load() {
	if (m_filename.empty()) {
		return;
	}

	Registry()[m_filename.extension().string()].first(this, m_filename);
}
}
