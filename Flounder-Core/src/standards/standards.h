#pragma once

#include <vector>

#include "../framework/framework.h"

#include "istandard.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing simple update injection standards.
	/// </summary>
	class standards :
		public imodule
	{
	private:
		std::vector<istandard*> *m_standards;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static standards *get()
		{
			return static_cast<standards*>(framework::get()->getInstance("standards"));
		}

		standards();

		~standards();

		void addStandard(istandard *standard);

		void init() override;

		void update() override;
	};
}
