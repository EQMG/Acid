#pragma once

#include <string>

namespace fl
{
	class FL_EXPORT IResource
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
