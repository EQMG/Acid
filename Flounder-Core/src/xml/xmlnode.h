#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace flounder
{
	/// <summary>
	/// Represents a node in an XML file. This contains the name of the node, a map of the attributes and their values, any text data between the start and end tag, and a list of all its children nodes.
	/// </summary>
	class xmlnode
	{
	private:
		std::string m_name;
		std::string m_data;
		std::map<std::string, std::string> *m_attributes;
		std::map<std::string, std::vector<xmlnode*>*> *m_childNodes;
	public:
		xmlnode(const std::string &name);

		~xmlnode();

		/// <summary>
		/// Gets the name of the XML node.
		/// </summary>
		/// <returns> The name of the XML node. </returns>
		inline std::string getName() { return m_name; }

		/// <summary>
		/// Gets any text data contained between the start and end tag of the node.
		/// </summary>
		/// <returns> The text data. </returns>
		inline std::string getData() { return m_data; }

		/// <summary>
		/// Gets the value of a certain attribute of the node. Returns {@code null} if the attribute doesn't exist.
		/// </summary>
		/// <param name="attribute"> The name of the attribute.
		/// </param>
		/// <returns> The value of the attribute. </returns>
		std::string getAttribute(const std::string &attribute);

		/// <summary>
		/// Gets a certain child node of this node.
		/// </summary>
		/// <param name="childName"> The name of the child node. </param>
		/// <returns> The child XML node with the given name. </returns>
		xmlnode *getChild(const std::string &childName);

		/// <summary>
		/// Gets a child node with a certain name, and with a given value of a given attribute.
		/// Used to get a specific child when there are multiple child nodes with the same node name.
		/// </summary>
		/// <param name="childName"> The name of the child node. </param>
		/// <param name="attribute"> The attribute whose value is to be checked. </param>
		/// <param name="value"> The value that the attribute must have. </param>
		/// <returns> The child node which has the correct name and the correct value for the chosen attribute. </returns>
		xmlnode *getChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value);

		/// <summary>
		/// Get the child nodes of this node that have a given name.
		/// </summary>
		/// <param name="name"> The name of the child nodes. </param>
		/// <returns> A list of the child nodes with the given name. If none exist then an empty list is returned. </returns>
		std::vector<xmlnode*> getChildren(const std::string &name);

		/// <summary>
		/// Sets some data for this node.
		/// </summary>
		/// <param name="data"> The data for this node (text that is found between the start and end tags of this node). </param>
		inline void setData(const std::string &data) { m_data = data; }

		/// <summary>
		/// Adds a new attribute to this node. An attribute has a name and a value.
		/// Attributes are stored in a HashMap which is initialized in here if it was previously null.
		/// </summary>
		/// <param name="attribute"> The name of the attribute. </param>
		/// <param name="value"> The value of the attribute. </param>
		void addAttribute(const std::string &attribute, const std::string &value);

		/// <summary>
		/// Adds a child node to this node.
		/// </summary>
		/// <param name="child"> The child node to add. </param>
		void addChild(xmlnode *child);
	};
}