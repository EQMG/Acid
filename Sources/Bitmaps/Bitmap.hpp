#pragma once

#include <unordered_map>
#include <functional>

#include "Maths/Vector2.hpp"

namespace acid {
template<typename Base>
class BitmapFactory {
public:
	using TLoadMethod = std::function<void(Base &, const std::filesystem::path &)>;
	using TWriteMethod = std::function<void(const Base &, const std::filesystem::path &)>;
	using TRegistryMap = std::unordered_map<std::string, std::pair<TLoadMethod, TWriteMethod>>;

	virtual ~BitmapFactory() = default;

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
				BitmapFactory::Registry()[name] = std::make_pair(&T::Load, &T::Write);
			return true;
		}
	};
};

class ACID_EXPORT Bitmap : public BitmapFactory<Bitmap> {
public:
	Bitmap() = default;
	explicit Bitmap(std::filesystem::path filename);
	explicit Bitmap(const Vector2ui &size, uint32_t bytesPerPixel = 4);
	Bitmap(std::unique_ptr<uint8_t[]> &&data, const Vector2ui &size, uint32_t bytesPerPixel = 4);
	~Bitmap() = default;

	void Load(const std::filesystem::path &filename);
	void Write(const std::filesystem::path &filename) const;

	explicit operator bool() const noexcept { return !data; }

	uint32_t GetLength() const;

	const std::filesystem::path &GetFilename() const { return filename; }
	void SetFilename(const std::filesystem::path &filename) { this->filename = filename; }

	const std::unique_ptr<uint8_t[]> &GetData() const { return data; }
	std::unique_ptr<uint8_t[]> &GetData() { return data; }
	void SetData(std::unique_ptr<uint8_t[]> &&data) { this->data = std::move(data); }

	const Vector2ui &GetSize() const { return size; }
	void SetSize(const Vector2ui &size) { this->size = size; }

	uint32_t GetBytesPerPixel() const { return bytesPerPixel; }
	void SetBytesPerPixel(uint32_t bytesPerPixel) { this->bytesPerPixel = bytesPerPixel; }

private:
	static uint32_t CalculateLength(const Vector2ui &size, uint32_t bytesPerPixel);
	
	std::filesystem::path filename;
	std::unique_ptr<uint8_t[]> data;
	Vector2ui size;
	uint32_t bytesPerPixel = 0;
};
}

