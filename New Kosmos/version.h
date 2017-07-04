#pragma once

#include <string>

/// <summary>
/// A timer implementation for events.
/// </summary>
class version
{
public:
	std::string m_version;

	/// <summary>
	/// Creates a new version.
	/// </summary>
	/// <param name="interval"> The version string. </param>
	version(std::string version);

	/// <summary>
	/// Deconstructor for the version.
	/// </summary>
	~version();
};
