#pragma once

namespace flounder {
	/// <summary>
	/// The object the factory will be loading into.
	/// </summary>
	class ifactoryobject 
	{
	private:
		bool m_dataLoaded;
		bool m_fullyLoaded;
	public:
		/// <summary>
		/// Creates a new empty factory object.
		/// </summary>
		ifactoryobject() 
		{
			m_dataLoaded = false;
			m_fullyLoaded = false;
		}

		/// <summary>
		/// Deconstructor for the factory object.
		/// </summary>
		~ifactoryobject()
		{
		}

		/// <summary>
		/// Gets if the data has been loaded into the object.
		/// </summary>
		/// <returns> If the object is loaded. </returns>
		bool isDataLoaded() { return m_dataLoaded; }

		/// <summary>
		/// Sets that the factory has data loaded.
		/// </summary>
		/// <param name="loaded"> If the data has been loaded. </param>
		void setDataLoaded(bool dataLoaded) { m_dataLoaded = dataLoaded; }

		/// <summary>
		/// Gets if the information has been loaded into the object.
		/// </summary>
		/// <returns> If the object is loaded. </returns>
		bool isLoaded() { return m_fullyLoaded; }

		/// <summary>
		/// Sets that the factory has been loaded.
		/// </summary>
		/// <param name="fullyLoaded"> If the factory has been loaded. </param>
		void setFullyLoaded(bool fullyLoaded) { m_fullyLoaded = fullyLoaded;  }
	};
}