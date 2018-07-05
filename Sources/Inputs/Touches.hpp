#pragma once

#include "Engine/Engine.hpp"
#include "Display/Display.hpp"
#include "Maths/Vector2.hpp"

namespace fl
{
	/// <summary>
	/// A definition for a managed touch input.
	/// </summary>
	struct Touch
	{
		uint32_t m_id;
		WsiAction m_action;
		float m_x;
		float m_y;
	};
	/// <summary>
	/// A module used for the creation, updating and destruction of touch inputs.
	/// </summary>
	class FL_EXPORT Touches :
		public IModule
	{
	private:
		std::array<Touch, 10> m_touches;

		friend void CallbackTouch(WsiShell shell, uint32_t id, float x, float y, WsiAction action);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Touches *Get()
		{
			return Engine::Get()->GetModule<Touches>();
		}

		/// <summary>
		/// Creates a new Touch module.
		/// </summary>
		Touches();

		/// <summary>
		/// Deconstructor for the Touch module.
		/// </summary>
		~Touches() override;

		void Update() override;

		/// <summary>
		/// Gets whether or not a particular location is currently pressed.
		/// <p>Actions: WSI_ACTION_PRESS, WSI_ACTION_RELEASE, WSI_ACTION_REPEAT</p>
		/// </summary>
		/// <param name="coord"> The location to test. </param>
		/// <param name="maxDistance"> How far away the error can be. </param>
		/// <returns> If the location is currently pressed. </returns>
		bool GetTouch(const Vector2 &coord, const float &maxDistance = 0.05f);
	};
}
