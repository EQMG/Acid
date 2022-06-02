#pragma once

#include <unordered_map>

#include "Maths/Vector3.hpp"
#include "Resources/Resource.hpp"
#include "Audio.hpp"

namespace acid {
template<typename Base>
class SoundBufferFactory {
public:
	using TLoadMethod = std::function<void(Base &, const std::filesystem::path &)>;
	using TWriteMethod = std::function<void(const Base &, const std::filesystem::path &)>;
	using TRegistryMap = std::unordered_map<std::string, std::pair<TLoadMethod, TWriteMethod>>;

	virtual ~SoundBufferFactory() = default;

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	template<typename T>
	class Registrar /*: public Base*/ {
	protected:
		template<typename ...Args>
		static bool Register(Args &&... names) {
			for (std::string &&name : {names...})
				SoundBufferFactory::Registry()[name] = std::make_pair(&T::Load, &T::Write);
			return true;
		}
	};
};

/**
 * @brief Resource that represents a sound buffer.
 */
class ACID_SOUND_EXPORT SoundBuffer : public SoundBufferFactory<SoundBuffer>, public Resource {
public:
	/**
	 * Creates a new sound buffer, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The sound buffer with the requested values.
	 */
	static std::shared_ptr<SoundBuffer> Create(const Node &node);
	/**
	 * Creates a new sound buffer, or finds one with the same values.
	 * @param filename The file to load the sound buffer from.
	 * @return The sound buffer with the requested values.
	 */
	static std::shared_ptr<SoundBuffer> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new sound buffer.
	 * @param filename The file to load the sound buffer from.
	 * @param load If this resource will be loaded immediately, otherwise {@link SoundBuffer#Load} can be called later.
	 */
	explicit SoundBuffer(std::filesystem::path filename, bool load = true);
	~SoundBuffer();

	std::type_index GetTypeIndex() const override { return typeid(SoundBuffer); }

	const std::filesystem::path &GetFilename() const { return filename; };
	uint32_t GetBuffer() const { return buffer; }
	void SetBuffer(uint32_t buffer);

	friend const Node &operator>>(const Node &node, SoundBuffer &soundBuffer);
	friend Node &operator<<(Node &node, const SoundBuffer &soundBuffer);

private:
	void Load();

	std::filesystem::path filename;
	uint32_t buffer = 0;
};
}
