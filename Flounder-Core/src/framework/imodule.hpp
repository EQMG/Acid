#pragma once

namespace flounder
{
	/// <summary>
	/// A simple interface used for defining framework modules.
	/// </summary>
	class imodule
	{
	public:
		/// <summary>
		/// Creates a new module.
		/// </summary>
		imodule()
		{
		}

		/// <summary>
		/// Deconstructor for the module.
		/// </summary>
		virtual ~imodule()
		{
		}

		/// <summary>
		/// The update function for the module.
		/// </summary>
		virtual void update() = 0;
	};
}
