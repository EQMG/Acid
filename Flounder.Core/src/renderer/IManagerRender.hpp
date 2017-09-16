#pragma once

namespace Flounder
{
	/// <summary>
	/// A extension used with irenderers to define a master renderer.
	/// </summary>
	class IManagerRender
	{
	public:
		/// <summary>
		/// Creates a new master renderer.
		/// </summary>
		IManagerRender()
		{
		}

		/// <summary>
		/// Deconstructor for the master renderer.
		/// </summary>
		virtual ~IManagerRender()
		{
		}

		virtual void CreateCommands(size_t i, std::vector<VkCommandBuffer> commandBuffers) = 0;

		/// <summary>
		/// Run when rendering the master renderer.
		/// </summary>
		virtual void Render() = 0;
	};
}
