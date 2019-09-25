#pragma once

#include "StdAfx.hpp"
#include "Maths/Vector2.hpp"

namespace acid {
template<typename Base>
class BitmapFactory {
public:
	using TLoadMethod = std::function<void(Base *, const std::filesystem::path &)>;
	using TWriteMethod = std::function<void(const Base *, const std::filesystem::path &)>;
	using TRegistryMap = std::unordered_map<std::string, std::pair<TLoadMethod, TWriteMethod>>;

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	template<typename T>
	class Registrar /*: public Base*/ {
	protected:
		static bool Register(const std::string &name) {
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

	explicit operator bool() const noexcept { return !m_data; }

	uint32_t GetLength() const;

	const std::filesystem::path &GetFilename() const { return m_filename; }
	void SetFilename(const std::filesystem::path &filename) { m_filename = filename; }

	const std::unique_ptr<uint8_t[]> &GetData() const { return m_data; }
	std::unique_ptr<uint8_t[]> &GetData() { return m_data; }
	void SetData(std::unique_ptr<uint8_t[]> &&data) { m_data = std::move(data); }

	const Vector2ui &GetSize() const { return m_size; }
	void SetSize(const Vector2ui &size) { m_size = size; }

	uint32_t GetBytesPerPixel() const { return m_bytesPerPixel; }
	void SetBytesPerPixel(uint32_t bytesPerPixel) { m_bytesPerPixel = bytesPerPixel; }

private:
	static uint32_t CalculateLength(const Vector2ui &size, uint32_t bytesPerPixel);
	
	std::filesystem::path m_filename;
	std::unique_ptr<uint8_t[]> m_data;
	Vector2ui m_size;
	uint32_t m_bytesPerPixel = 0;
};
}

