#pragma once

/// <summary>
/// A reference to a time fetching function
/// </summary>
class timingref
{
public:
	/// <summary>
	/// Gets the time from the function.
	/// </summary>
	/// <returns> The time read in seconds. </returns>
	virtual double getTime() = 0;
};
