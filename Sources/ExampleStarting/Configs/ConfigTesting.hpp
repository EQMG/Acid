#pragma once

#include <Config/Config.hpp>

using namespace Flounder;

namespace Demo
{
	class ConfigTesting :
		public FileXml
	{
	public:
		float x;
		float y;
		double z;
		int p;
		std::string meme;

		ConfigTesting() :
			FileXml("Configs/Testing.json")
		{
		}

		~ConfigTesting()
		{
		}
	protected:
		void Action(const ActionType &type) override
		{
			ConfigRequest<float>(type, "x", 2.0f, &x);
			ConfigRequest<float>(type, "y", -1.3f, &y);
			ConfigRequest<double>(type, "z", 0.987, &z);
			ConfigRequest<int>(type, "p", 190, &p);
			ConfigRequest<std::string>(type, "Meme", "Rare Pepe", &meme);
			printf("x: %f, meme: %s", x, meme.c_str());
		}
	};
}