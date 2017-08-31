#pragma once

namespace Flounder
{
	/// <summary>
	/// Interface for executable requests.
	/// </summary>
	class irequest
	{
	public:
		/// <summary>
		/// Creates a new request.
		/// </summary>
		irequest()
		{
		}

		/// <summary>
		/// Deconstructor for the request.
		/// </summary>
		virtual ~irequest()
		{
		}

		/// <summary>
		/// Gets the request id.
		/// </summary>
		/// <returns> The request id used. </returns>
		virtual int getRequestId() = 0;
	};
}
