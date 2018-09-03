#pragma once

#include <Files/Config.hpp>

using namespace acid;

namespace test
{
	class ConfigManager :
		public Config
	{
	private:
	public:
		ConfigManager();

		~ConfigManager();

		void Decode(Serialized &serialized) override;

		void Encode(Serialized &serialized) const override;
	};
}