#pragma once

namespace flounder {
	/// <summary>
	/// A class used to define how the framework will run updates and timings.
	/// </summary>
	class updater {
	public:
		/// <summary>
		/// Function called by the framework to initialize and start the game loop used for updating and profiling.
		/// </summary>
		virtual void run() = 0;

		/// <summary>
		/// Disposed the framework if initialised.
		/// </summary>
		virtual void dispose() = 0;

		/// <summary>
		/// Gets the added/removed time for the framework (seconds).
		/// </summary>
		/// <returns> The time offset. </returns>
		virtual float getTimeOffset() = 0;

		/// <summary>
		/// Sets the time offset for the framework (seconds).
		/// </summary>
		/// <param name="timeOffset"> The new time offset. </param>
		virtual void setTimeOffset(float) = 0;

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		virtual float getDelta() = 0;

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		virtual float getDeltaRender() = 0;

		virtual void setFpsLimit(float) = 0;

		/// <summary>
		/// Gets the current time of the framework instance.
		/// </summary>
		/// <returns> The current framework time in seconds. </returns>
		virtual float getTimeSec() = 0;

		/// <summary>
		/// Gets the current time of the framework instance.
		/// </summary>
		/// <returns> The current framework time in milliseconds. </returns>
		virtual float getTimeMs() = 0;
	};
}
