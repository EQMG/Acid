#pragma once

namespace flounder {
	/// <summary>
	/// A simple task runner.
	/// </summary>
	class itask {
	public:
		/// <summary>
		/// Run when the task is executed.
		/// </summary>
		virtual void execute() = 0;
	};
}