#pragma once

#include "StdAfx.hpp"

namespace acid {
struct Version {
	uint32_t m_major, m_minor, m_patch;
};

/**
 * @brief Class that represents a application.
 */
class ACID_EXPORT App {
	friend class Engine;
public:
	explicit App(std::string name, const Version &version = {1, 0, 0}) :
		m_name(std::move(name)),
		m_version(version) {
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
	const std::string &GetName() const { return m_name; }

	/**
	 * Sets the application's name, for driver support.
	 * @param name The application's name.
	 */
	void SetName(const std::string &name) { m_name = name; }

	/**
	 * Gets the application's version.
	 * @return The application's version.
	 */
	const Version &GetVersion() const { return m_version; }

	/**
	 * Sets the application's version, for driver support.
	 * @param version The application version.
	 */
	void SetVersion(const Version &version) { m_version = version; }

private:
	bool m_started = false;
	std::string m_name;
	Version m_version;
};
}
