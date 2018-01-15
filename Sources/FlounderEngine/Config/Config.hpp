#pragma once

#include <string>
#include <cassert>

namespace Flounder
{
	class FileXml
	{
	private:
		std::string m_filename;
	public:
		enum ActionType
		{
			ActionRead = 0,
			ActionWrite = 1
		};

		FileXml(const std::string &filename);

		~FileXml();

		void ReadConfig();

		void WriteConfig();
	protected:
		template<typename T>
		void ConfigRequest(const ActionType &type, std::string name, T defaultValue, T* pointer)
		{
			switch (type)
			{
				case ActionRead:
					T* read = nullptr; // TODO Read from config buffer by name.

					if (read == nullptr)
					{
						read = &defaultValue;
					}

					pointer = read;
					break;
				case ActionWrite:
					// TODO: Write to config buffer by name.
					break;
				default:
					assert(false && "Invalid config request type!");
					break;
			}
		}

		virtual void Action(const ActionType &type) = 0;
	};
}
