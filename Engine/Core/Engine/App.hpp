#pragma once

#include <rocket.hpp>

#include "CoreExport.hpp"

#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif

namespace acid {
class ACID_CORE_EXPORT Version {
public:
	Version(uint8_t major, uint8_t minor, uint8_t patch) :
		major(major),
		minor(minor),
		patch(patch) {
	}

	uint8_t major, minor, patch;
};

/**
 * @brief Class that represents a application.
 */
class ACID_CORE_EXPORT App : public virtual rocket::trackable {
	friend class Engine;
public:
	explicit App(std::string name, const Version &version = {1, 0, 0}) :
		name(std::move(name)),
		version(version) {
	}

	virtual ~App() = default;

	/**
	 * Run when switching to this app from another.
	 */
	virtual void Start() = 0;

	/**
	 * Run before the module update pass.
	 */
	virtual void Update() = 0;

	/**
	 * Gets the application's name.
	 * @return The application's name.
	 */
	const std::string &GetName() const { return name; }

	/**
	 * Sets the application's name, for driver support.
	 * @param name The application's name.
	 */
	void SetName(const std::string &name) { this->name = name; }

	/**
	 * Gets the application's version.
	 * @return The application's version.
	 */
	const Version &GetVersion() const { return version; }

	/**
	 * Sets the application's version, for driver support.
	 * @param version The application version.
	 */
	void SetVersion(const Version &version) { this->version = version; }

private:
	bool started = false;
	std::string name;
	Version version;
};
}
