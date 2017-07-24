#include "xmlparser.h"

namespace flounder
{
//	pattern *const xmlparser::DATA = pattern::compile(">(.+?)<");
//	pattern *const xmlparser::START_TAG = pattern::compile("<(.+?)>");
//	pattern *const xmlparser::ATTR_NAME = pattern::compile("(.+?)=");
//	pattern *const xmlparser::ATTR_VALUE = pattern::compile("\"(.+?)\"");
//	pattern *const xmlparser::CLOSED = pattern::compile("(</|/>)");

	xmlnode *xmlparser::loadXmlFile(const std::string filepath)
	{
		std::string fileLoaded = helperfile::readFile(filepath);
		std::vector<std::string> lines = helperstring::split(fileLoaded, "\n");
		return loadNode(0, lines);
	}

	xmlnode *xmlparser::loadNode(const int &progress, std::vector<std::string> lines)
	{
		std::string line = lines.at(progress);
		
		if (helperstring::startsWith(line, "</"))
		{
			return NULL;
		}

		std::vector<std::string> startTagParts = helperstring::split(getStartTag(line), " ");

		xmlnode *node = new xmlnode(helperstring::replace(startTagParts.at(0), "/", ""));
		addAttributes(startTagParts, node);
		addData(line, node);

	//	if (CLOSED->matcher(line)->find())
	//	{
	//		return node;
	//	}

		xmlnode *child;

		while ((child = loadNode(progress + 1, lines)) != 0)
		{
			node->addChild(child);
		}

		return node;
	}

	void xmlparser::addData(const std::string &line, xmlnode *node)
	{
	//	matcher *dataMatch = DATA->matcher(line);

	//	if (dataMatch->find())
	//	{
	//		node->setData(dataMatch->group(1));
	//	}
	}

	void xmlparser::addAttributes(std::vector<std::string> titleParts, xmlnode *node)
	{
		for (int i = 1; i < titleParts.size(); i++)
		{
			if (titleParts.at(i).find("=") != std::string::npos)
			{
				addAttribute(titleParts[i], node);
			}
		}
	}

	void xmlparser::addAttribute(const std::string &attributeLine, xmlnode *node)
	{
	//	matcher *nameMatch = ATTR_NAME->matcher(attributeLine);
	//	nameMatch->find();
	//	matcher *valueMatch = ATTR_VALUE->matcher(attributeLine);
	//	valueMatch->find();
	//	node->addAttribute(nameMatch->group(1), valueMatch->group(1));
	}

	std::string xmlparser::getStartTag(const std::string &line)
	{
		return std::string();
	}
}
