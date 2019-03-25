#pragma once

#include <thread>
#include "Maths/Time.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
	/// <summary>
	/// A class that can listen to file changes on a path recursively.
	/// </summary>
	class ACID_EXPORT FileWatcher
	{
	public:
		enum class Status
		{
			Created, Modified, Erased
		};

		/// <summary>
		/// Creates a new file watcher.
		/// </summary>
		/// <param name="path"> The path to watch recursively. </param>
		/// <param name="delay"> How frequently to check for changes. </param>
		explicit FileWatcher(std::string path, const Time &delay = Time::Seconds(5.0f));

		~FileWatcher();

		const std::string &GetPath() const { return m_path; }

		void SetPath(const std::string &path) { m_path = path; }

		const Time &GetDelay() const { return m_delay; }

		void SetDelay(const Time &delay) { m_delay = delay; }

		Delegate<void(std::string, Status)> &GetOnChange() { return m_onChange; }
	private:
		void QueueLoop();

		bool Contains(const std::string &key) const;

		std::string m_path;
		Time m_delay;
		Delegate<void(std::string, Status)> m_onChange;

		std::thread m_thread;
		std::unordered_map<std::string, long> m_paths;
		bool m_running;
	};
}
