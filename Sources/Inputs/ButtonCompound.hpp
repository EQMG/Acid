#pragma once

#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "IButton.hpp"

namespace acid
{
	/// <summary>
	/// Handles multiple buttons at once.
	/// </summary>
	class ACID_EXPORT ButtonCompound :
		public IButton,
		public NonCopyable
	{
	public:
		/// <summary>
		/// A template used to create a compound button of a single type.
		/// </summary>
		/// <param name="args"> The arguments to pass to T. </param>
		/// <param name="T"> The type of buttons to create. </param>
		/// <param name="Args"> The values passed to each button. </param>
		template<class T, typename... Args>
		static ButtonCompound *Create(Args &&... args)
		{
			std::vector<IButton *> buttons;

			for (const auto &x : {args...})
			{
				buttons.emplace_back(new T(x));
			}

			return new ButtonCompound(buttons);
		}

		/// <summary>
		/// Creates a new compound button.
		/// </summary>
		/// <param name="buttons"> The buttons on the being added. </param>
		/// <param name="useAnd"> If <seealso cref="#IsDown()"/> will check if all buttons are down instead of just one. </param>
		explicit ButtonCompound(const std::vector<IButton *> &buttons, const bool &useAnd = false);

		bool IsDown() const override;

		bool WasDown() override;
	private:
		std::vector<std::unique_ptr<IButton>> m_buttons;
		bool m_useAnd;
		bool m_wasDown;
	};
}
