#pragma once

#include <string>
#include <vector>
#include "Engine/Exports.hpp"

namespace acid
{
	class ACID_EXPORT DataStream
	{
	public:
		/// A bool-like type that cannot be converted to integer or pointer types.
		typedef bool (DataStream::*BoolType)(const std::size_t&);

		/// <summary>
		/// Default constructor, creates an empty data stream.
		/// </summary>
		DataStream();

		/// <summary>
		/// Append data to the end of the packet.
		/// </summary>
		/// <param name="data"> Pointer to the sequence of bytes to append. </param>
		/// <param name="sizeInBytes"> Number of bytes to append. </param>
		void Append(const void *data, const std::size_t &sizeInBytes);

		/// <summary>
		/// Clear the packet, after calling Clear, the packet is empty.
		/// </summary>
		void Clear();

		/// <summary>
		/// Get a pointer to the data contained in the packet.
		/// Warning: the returned pointer may become invalid after  you append data to the packet,
		/// therefore it should never be stored.
		/// The return pointer is null if the packet is empty.
		/// </summary>
		/// <returns> Pointer to the data. </returns>
		const void *GetData() const;

		/// <summary>
		/// Get the size of the data contained in the packet.
		/// This function returns the number of bytes pointed to by what getData returns.
		/// </summary>
		/// <returns> Data size, in bytes. </returns>
		std::size_t GetDataSize() const;

		/// <summary>
		/// Tell if the reading position has reached the end of the packet.
		/// This function is useful to know if there is some data left to be read, without actually reading it.
		/// </summary>
		/// <returns> True if all data was read, false otherwise. </returns>
		bool EndOfStream() const;

		/// <summary>
		/// Test the validity of the packet, for reading.
		/// This operator allows to test the packet as a boolean variable, to check if a reading operation was successful.
		/// A packet will be in an invalid state if it has no more data to read. This behavior is the same as standard C++ streams.
		///
		/// Usage example:
		/// <code>
		/// float x;
		/// packet >> x;
		/// if (packet)
		/// {
		/// // ok, x was extracted successfully
		/// }
		///
		/// // -- or --
		///
		/// float x;
		/// if (packet >> x)
		/// {
		/// // ok, x was extracted successfully
		/// }
		/// </code>
		///
		/// Don't focus on the return type, it's equivalent to bool but it disallows unwanted implicit conversions to integer or pointer types.
		/// </summary>
		/// <returns> True if last data extraction from packet was successful. </returns>
		operator BoolType() const;

		// Overload of operator >> to read data from the data stream

		DataStream &operator>>(bool &data);

		DataStream &operator>>(int8_t &data);

		DataStream &operator>>(uint8_t &data);

		DataStream &operator>>(int16_t &data);

		DataStream &operator>>(uint16_t &data);

		DataStream &operator>>(int32_t &data);

		DataStream &operator>>(uint32_t &data);

		DataStream &operator>>(int64_t &data);

		DataStream &operator>>(uint64_t &data);

		DataStream &operator>>(float &data);

		DataStream &operator>>(double &data);

		DataStream &operator>>(char *data);

		DataStream &operator>>(std::string &data);

		DataStream &operator>>(wchar_t *data);

		DataStream &operator>>(std::wstring &data);

		// Overload of operator << to write data into the data stream

		DataStream &operator<<(const bool &data);

		DataStream &operator<<(const int8_t &data);

		DataStream &operator<<(const uint8_t &data);

		DataStream &operator<<(const int16_t &data);

		DataStream &operator<<(const uint16_t &data);

		DataStream &operator<<(const int32_t &data);

		DataStream &operator<<(const uint32_t &data);

		DataStream &operator<<(const int64_t &data);

		DataStream &operator<<(const uint64_t &data);

		DataStream &operator<<(const float &data);

		DataStream &operator<<(const double &data);

		DataStream &operator<<(const char *data);

		DataStream &operator<<(const std::string &data);

		DataStream &operator<<(const wchar_t *data);

		DataStream &operator<<(const std::wstring &data);
	protected:
		/// <summary>
		/// Check if the packet can extract a given number of bytes.
		/// This function updates accordingly the state of the packet.
		/// </summary>
		/// <param name="size"> Size to check. </param>
		/// <returns> True if \a size bytes can be read from the packet. </returns>
		bool CheckSize(const std::size_t &size);

		/// Data stored in the packet.
		std::vector<char> m_data;
		/// Current reading position in the packet.
		std::size_t m_readPos;
		/// Current send position in the packet (for handling partial sends).
		std::size_t m_sendPos;
		/// Reading state of the packet.
		bool m_isValid;
	};
}
