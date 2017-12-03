#pragma once

namespace Flounder
{
	/// <summary>
	/// Interface for executable requests.
	/// </summary>
	class IRequest
	{
	public:
		/// <summary>
		/// Creates a new request.
		/// </summary>
		IRequest()
		{
		}

		/// <summary>
		/// Deconstructor for the request.
		/// </summary>
		virtual ~IRequest()
		{
		}

		/// <summary>
		/// Gets the request id.
		/// </summary>
		/// <returns> The request id used. </returns>
		virtual int GetRequestId() = 0;
	};
}
