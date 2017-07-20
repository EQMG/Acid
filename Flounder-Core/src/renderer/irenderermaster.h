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
		/// Run when rendering the master renderer.
		/// </summary>
		virtual void render() = 0;
	};
}