#pragma once

#include "StdAfx.hpp"
#include "Maths/Vector2.hpp"

namespace acid {
template<typename Base>
class BitmapFactory {
public:
	using TCreateReturn = std::unique_ptr<Base>;

	using TCreateMethod = std::function<TCreateReturn()>;
	using TRegistryMap = std::unordered_map<std::string, TCreateMethod>;

	static TCreateReturn Create(const std::string &name) {
		auto it = Registry().find(name);
		return it == Registry().end() ? nullptr : it->second();
	}

	static TCreateReturn Create(const std::filesystem::path &filename) {
		return Create(filename.extension().string());
	}

	static TCreateReturn Load(const std::filesystem::path &filename) {
		auto it = Registry().find(filename.extension().string());
		return it == Registry().end() ? nullptr : it->second();
	}

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	template<typename T>
	class Registrar : public Base {
	protected:
		static bool Register(const std::string &name) {
			BitmapFactory::Registry()[name] = []() -> TCreateReturn {
				return std::make_unique<T>();
			};
			return true;
		}
	};
};

class ACID_EXPORT Bitmap : public BitmapFactory<Bitmap> {
public:
	Bitmap() = default;
	Bitmap(const Vector2ui &size, uint32_t bytesPerPixel = 4) :
		m_size(size),
		m_bytesPerPixel(bytesPerPixel) {
	}
	Bitmap(std::vector<uint8_t> data, const Vector2ui &size, uint32_t bytesPerPixel = 4) :
		m_data(std::move(data)),
		m_size(size),
		m_bytesPerPixel(bytesPerPixel) {
	}

	virtual ~Bitmap() = default;

	virtual void Load(const std::filesystem::path &filename) {}
	virtual void Write(const std::filesystem::path &filename) const {}

	constexpr explicit operator bool() const noexcept { return !m_data.empty(); }
	
	std::vector<uint8_t> m_data;
	Vector2ui m_size;
	uint32_t m_bytesPerPixel = 0;
};
}

