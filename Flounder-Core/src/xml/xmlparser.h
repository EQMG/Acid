#pragma once

#include <string>
#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"
#include "xmlnode.h"

namespace flounder
{
	/// <summary>
	/// Reads an XML file and stores all the data in <seealso cref="xmlnode"/> objects, allowing for easy access to the data contained in the XML file.
	/// </summary>
	class xmlparser
	{
	private:
	//	static pattern *const DATA;
	//	static pattern *const START_TAG;
	//	static pattern *const ATTR_NAME;
	//	static pattern *const ATTR_VALUE;
	//	static pattern *const CLOSED;
	public:
		/// <summary>
		/// Reads an XML file and stores all the data in <seealso cref="XmlNode"/> objects, allowing for easy access to the data contained in the XML file.
		/// </summary>
		/// <param name="filepath"> The XML file </param>
		/// <returns> The root node of the XML structure. </returns>
		static xmlnode *loadXmlFile(const std::string filepath);
	private:
		static xmlnode *loadNode(const int &progress, std::vector<std::string> lines);

		static void addData(const std::string &line, xmlnode *node);

		static void addAttributes(std::vector<std::string> titleParts, xmlnode *node);

		static void addAttribute(const std::string &attributeLine, xmlnode *node);
		
		static std::string getStartTag(const std::string &line);
	};
}
