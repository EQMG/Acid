#pragma once

#include <string>

namespace acid
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

		virtual std::string GetName() = 0;
	};
}
