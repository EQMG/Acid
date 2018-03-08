#pragma once

#include <string>
#include "../IFile.hpp"
#include "XmlNode.hpp"

// TODO: https://github.com/TheThinMatrix/OpenGL-Animation/tree/master/ColladaParser/xmlParser

namespace Flounder
{
	class F_EXPORT FileXml :
		public IFile
	{
	private:
		std::string m_filename;
		XmlNode *m_parentNode;
	public:
		FileXml(const std::string &filename);

		~FileXml();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::map<std::string, std::string> ConfigReadValues() override;

		void ConfigPushValue(const std::string &key, const std::string &value) override;

		XmlNode *GetParentNode() { return m_parentNode; }
	private:
		void Verify();

		std::string GetStartTag(const std::string &line);
	};
}