#pragma once

#include "ievent.h"

namespace flounder {
	template<typename t>
	class ieventchange : 
		public ievent
	{
	private:
		t *m_reference;
		t m_current;
	public:
		/// <summary>
		/// Creates a new change event.
		/// </summary>
		/// <param name="reference"> The reference to listen to. </param>
		ieventchange(t *reference)
		{
			m_reference = reference;
		}

		bool eventTriggered() override
		{
			t newValue = &m_reference;

			if (newValue == NULL)
			{
				return false;
			}

			bool triggered = newValue != m_current;
			m_current = newValue;
			return triggered;
		}

		virtual void onEvent() override = 0;

		inline bool removeAfterEvent() override { return false; }
	};
}
