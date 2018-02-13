#pragma once

#include <string>

namespace Flounder
{
	class F_EXPORT IResource
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
