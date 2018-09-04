#include "Serialize.hpp"

#include <ostream>

namespace acid
{
	std::string Serialize::Encode(const Node &node)
	{
		std::stringstream result;
		EncodeStream(node, result);
		return result.str();
	}

	Node Serialize::Decode(const std::string &value)
	{
	//	ColliderCapsule,[(Radius,0.2,3),(Height,1.8,3),],5
	//	Rigidbody,[(Mass,1.0,3),(Friction,0.4,3),(Linear_Factor,[(x,0.0,3),(y,0.0,3),(z,0.0,3),],5),(Angular_Factor,[(x,0.0,3),(y,0.0,3),(z,0.0,3),],5),],5
	//	FpsPlayer,1
	//	Mesh,[(Model,"Sphere_10_10_1.0",3),],5
		return Node(); // TODO: Implement.
	}

	void Serialize::EncodeStream(const Node &node, std::stringstream &ss)
	{
		EncodePropsFlags props = PROP_NONE;

		if (!node.GetName().empty())
		{
			ss << node.GetName() << ",";
			props |= PROP_NAME;
		}

		if (!node.GetValue().empty())
		{
			ss << node.GetValue() << ",";
			props |= PROP_VALUE;
		}

		if (node.GetChildCount() != 0)
		{
			ss << "[";

			for (auto &child : node.GetChildren())
			{
				ss << "(";
				EncodeStream(*child, ss);
				ss << "),";
			}

			ss << "],";
			props |= PROP_CHILDREN;
		}

		if (node.GetAttributeCount() != 0)
		{
			ss << "[";

			for (auto &attribute : node.GetAttributes())
			{
				ss << attribute.first << "," << attribute.second;
			}

			ss << "],";
			props |= PROP_ATTRIBUTES;
		}

		ss << props;
	}
}
