#pragma once

namespace flounder
{
	/// <summary>
	/// A extension used with irenderers to define a master renderer.
	/// </summary>
	class irenderermaster
	{
	public:
		/// <summary>
		/// Creates a new master renderer.
		/// </summary>
		irenderermaster()
		{
		}

		/// <summary>
		/// Deconstructor for the master renderer.
		/// </summary>
		virtual ~irenderermaster()
		{
		}

		/// <summary>
		/// Run when rendering the master renderer.
		/// </summary>
		virtual void render() = 0;
	};
}
