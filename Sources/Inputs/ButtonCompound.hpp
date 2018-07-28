#pragma once

#include <vector>
#include "IButton.hpp"

namespace acid
{
	/// <summary>
	/// Handles multiple buttons at once.
	/// </summary>
	class ACID_EXPORT ButtonCompound :
		public IButton
	{
	private:
		std::vector<IButton *> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new compound button.
		/// </summary>
		/// <param name="n_args"> The number buttons being added. </param>
		/// <param name="..."> The buttons on the being added. </param>
		ButtonCompound(const std::vector<IButton *> &buttons);

		/// <summary>
		/// Deconstructor for the compound button.
		/// </summary>
		~ButtonCompound();

		bool IsDown() const override;

		bool WasDown() override;
	};
}
