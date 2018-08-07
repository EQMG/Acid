#pragma once

#include <string>

namespace acid
{
	class ACID_EXPORT IResource
	{
	private:
	public:
		IResource()
		{
		}

		virtual ~IResource()
		{
		}

		virtual std::string GetFilename() = 0;
	};
}
