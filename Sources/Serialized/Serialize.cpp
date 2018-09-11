#include "Serialize.hpp"

#include <ostream>

namespace acid
{
	std::string Serialize::Encode(const Metadata &metadata)
	{
		std::stringstream result;
		EncodeStream(metadata, result);
		return result.str();
	}

	Metadata Serialize::Decode(const std::string &value)
	{
	//	ColliderCapsule,[(Radius,0.2,3),(Height,1.8,3),],5
	//	Rigidbody,[(Mass,1.0,3),(Friction,0.4,3),(Linear_Factor,[(x,0.0,3),(y,0.0,3),(z,0.0,3),],5),(Angular_Factor,[(x,0.0,3),(y,0.0,3),(z,0.0,3),],5),],5
	//	FpsPlayer,1
	//	Mesh,[(Model,"Sphere_10_10_1.0",3),],5
		return Metadata(); // TODO: Implement.
	}

	void Serialize::EncodeStream(const Metadata &metadata, std::stringstream &ss)
	{
		EncodePropsFlags props = PROP_NONE;

		if (!metadata.GetName().empty())
		{
			ss << metadata.GetName() << ",";
			props |= PROP_NAME;
		}

		if (!metadata.GetValue().empty())
		{
			ss << metadata.GetValue() << ",";
			props |= PROP_VALUE;
		}

		if (metadata.GetChildCount() != 0)
		{
			ss << "[";

			for (auto &child : metadata.GetChildren())
			{
				ss << "(";
				EncodeStream(*child, ss);
				ss << "),";
			}

			ss << "],";
			props |= PROP_CHILDREN;
		}

		if (metadata.GetAttributeCount() != 0)
		{
			ss << "[";

			for (auto &[name, value] : metadata.GetAttributes())
			{
				ss << name << "," << value;
			}

			ss << "],";
			props |= PROP_ATTRIBUTES;
		}

		ss << props;
	}
}
