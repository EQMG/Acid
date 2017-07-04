#pragma once

#include <iostream>
#include <algorithm>
#include "extension.h"

/// <summary>
/// A simple interface used for defining framework modules.
/// </summary>
class module {
protected:
	std::vector<module*> m_dependencies;
	std::vector<extension*> m_extensions;
	bool m_extensionChange;
public:
	static enum updateflag {
		ALWAYS, BEFORE, AFTER
	};

	module::updateflag flag;

	/// <summary>
	/// Creates a new module object.
	/// </summary>
	/// <param name="dependencies"> The list of module classes this module depends on. </param>
	module(module::updateflag flag, std::vector<module*> dependencies)
	{
		this->flag = flag;
		this->m_dependencies = dependencies;
		this->m_extensions = std::vector<extension*>();
		this->m_extensionChange = true;
	}

	/// <summary>
	/// Deconstructor for the module.
	/// </summary>
	~module() {
		delete &m_dependencies;
		delete &m_extensions;
	}

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void dispose() = 0;

	module::updateflag getFlag() {
		return flag;
	}

	/// <summary>
	/// Registers an extension with a module.
	/// </summary>
	/// <param name="extension"> The extension to init. </param>
	void registerExtension(extension *e)
	{
		if (!(std::find(m_extensions.begin(), m_extensions.end(), e) != m_extensions.end()))
		{
			// Framework->get()->registerModules(Framework->get()->loadModules(e->getDependencies())); // TODO
			m_extensions.push_back(e);
			m_extensionChange = true;
		}
	}
	/// <summary>
	/// Registers extensions with a module.
	/// </summary>
	/// <param name="extensions"> The extensions to init. </param>
	void registerExtensions(std::vector<extension*> extensions)
	{
		for (int i = 0; i < extensions.size(); i++) {
			registerExtension(extensions.at(i));
		}
	}
};
