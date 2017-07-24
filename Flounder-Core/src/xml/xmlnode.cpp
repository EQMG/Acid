#include "xmlnode.h"

namespace flounder
{
	xmlnode::xmlnode(const std::string & name)
	{
		m_name = name;
	}

	xmlnode::~xmlnode()
	{
		for (std::map<std::string, std::vector<xmlnode*>*>::iterator child = m_childNodes->begin(); child != m_childNodes->end(); ++child)
		{
			delete &*child;
		}

		delete m_attributes;
		delete m_childNodes;
	}

	std::string xmlnode::getAttribute(const std::string &attribute)
	{
		if (m_attributes != NULL)
		{
			return m_attributes->at(attribute);
		}
		else
		{
			return NULL;
		}
	}

	xmlnode * xmlnode::getChild(const std::string &childName)
	{
		if (m_childNodes != NULL)
		{
			std::vector<xmlnode*> *nodes = m_childNodes->at(childName);

			if (nodes->size() > 0 && !nodes->empty())
			{
				return nodes->at(0);
			}
		}

		std::cerr << "Could not find Xml node child: " << childName << " in parent " << m_name << std::endl;
		return NULL;
	}

	xmlnode * xmlnode::getChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value)
	{
		std::vector<xmlnode*> children = getChildren(childName);

		if (children.empty() || children.empty())
		{
			return NULL;
		}

		for (std::vector<xmlnode*>::const_iterator child = children.begin(); child != children.end(); ++child)
		{
			std::string val = (*child)->getAttribute(attribute);

			if (value == val)
			{
				return *child;
			}
		}

		std::cerr << "Could not find Xml node child: " << childName << " in parent " << m_name << " with attribute " << attribute << std::endl;
		return NULL;
	}

	std::vector<xmlnode*> xmlnode::getChildren(const std::string &name)
	{
		if (m_childNodes != NULL)
		{
			std::vector<xmlnode*> *children = m_childNodes->at(name);

			if (children->size() > 0)
			{
				return *children;
			}
		}

		return std::vector<xmlnode*>();
	}

	void xmlnode::addAttribute(const std::string &attribute, const std::string &value)
	{
		if (m_attributes == NULL)
		{
			m_attributes = new std::map<std::string, std::string>();
		}

		m_attributes->insert(attribute, value);
	}

	void xmlnode::addChild(xmlnode *child)
	{
		if (m_childNodes == NULL)
		{
			m_childNodes = new std::map<std::string, std::vector<xmlnode*>*>;
		}

	//	std::vector<xmlnode*> *list = m_childNodes->at(child->getName());
		
	//	if (list == NULL)
	//	{
	//		list = new std::vector<xmlnode*>();
	//		m_childNodes->insert(child->getName(), list);
	//	}

	//	list->push_back(child);
	}
}