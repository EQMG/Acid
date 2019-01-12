#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT JsonSection :
		public NonCopyable
	{
	private:
		friend class Json;
		JsonSection *m_parent;
		std::vector<std::unique_ptr<JsonSection>> m_children;

		std::string m_name;
		std::string m_content;
	public:
		JsonSection(JsonSection *parent, const std::string &name, const std::string &content);

		static void AppendData(const Metadata &source, std::stringstream &builder, const int32_t &indentation, const bool &end = false);

		static Metadata *Convert(const JsonSection &source, Metadata *parent, const bool &isTopSection = true);
	};
}
