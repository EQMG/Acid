#pragma once

namespace Flounder
{
	/// <summary>
	/// A simple interface used for defining engine modules.
	/// </summary>
	class IModule
	{
	public:
		/// <summary>
		/// Creates a new module.
		/// </summary>
		IModule()
		{
		}

		/// <summary>
		/// Deconstructor for the module.
		/// </summary>
		virtual ~IModule()
		{
		}

		/// <summary>
		/// The update function for the module.
		/// </summary>
		virtual void Update() = 0;
	};
}
