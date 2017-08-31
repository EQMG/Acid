#pragma once

namespace Flounder
{
	/// <summary>
	/// A extension used with standards to define a standards.
	/// </summary>
	class IStandard
	{
	public:
		/// <summary>
		/// Creates a new request.
		/// </summary>
		IStandard()
		{
		}

		/// <summary>
		/// Deconstructor for the standard.
		/// </summary>
		virtual ~IStandard()
		{
		}

		/// <summary>
		/// Run when updating the standards.
		/// </summary>
		virtual void Update() = 0;
	};
}
