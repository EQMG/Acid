#pragma once

namespace flounder
{
	/// <summary>
	/// A extension used with irenderers to define a master renderer.
	/// </summary>
	class imanagerrender
	{
	public:
		/// <summary>
		/// Creates a new master renderer.
		/// </summary>
		imanagerrender()
		{
		}

		/// <summary>
		/// Deconstructor for the master renderer.
		/// </summary>
		virtual ~imanagerrender()
		{
		}

		/// <summary>
		/// Run when rendering the master renderer.
		/// </summary>
		virtual void render() = 0;
	};
}
