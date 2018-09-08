#pragma once

#include <memory>
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
		std::vector<std::unique_ptr<IButton>> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new compound button.
		/// </summary>
		/// <param name="buttons"> The buttons on the being added. </param>
		ButtonCompound(const std::vector<IButton *> &buttons);

		~ButtonCompound();

		ButtonCompound(const ButtonCompound&) = delete; // FIXME: Temp Fix.

		ButtonCompound& operator=(const ButtonCompound&) = delete;

		bool IsDown() const override;

		bool WasDown() override;
	};
}
