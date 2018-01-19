#pragma once

#include <string>

namespace Flounder
{
	class IResource
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
