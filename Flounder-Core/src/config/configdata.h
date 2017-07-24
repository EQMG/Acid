#pragma once

#include <functional>
#include <string>

#include "../helpers/helperstring.h"

namespace flounder
{
	/// <summary>
	/// Config data that has been loaded from data or a config.
	/// </summary>
	template<typename t>
	class configdata
	{
	private:
		std::string m_key;
		std::string m_data;
		t *m_reference;
	public:
		configdata(const std::string &key, const std::string &data, t *reference);

		/// <summary>
		/// Gets the parsed data (String).
		/// </summary>
		/// <returns> The parsed data. </returns>
		std::string getString();

		/// <summary>
		/// Gets the parsed data (Boolean).
		/// </summary>
		/// <returns> The parsed data. </returns>
		bool getBool();

		/// <summary>
		/// Gets the parsed data (Integer).
		/// </summary>
		/// <returns> The parsed data. </returns>
		int getInt();

		/// <summary>
		/// Gets the parsed data (Double).
		/// </summary>
		/// <returns> The parsed data. </returns>
		double getDouble();

		/// <summary>
		/// Gets the parsed data (Float).
		/// </summary>
		/// <returns> The parsed data. </returns>
		float getFloat();

		/// <summary>
		/// Sets the reference to the data, used for saving.
		/// </summary>
		/// <param name="reference"> The new reference.
		/// </param>
		/// <returns> this. </returns>
		inline configdata *setReference(t *reference) { m_reference = reference; }
	private:
		std::string fixDataString(const std::string &string);
	};
}
