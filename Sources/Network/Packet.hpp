#pragma once

#include <string>
#include <vector>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Packets provide a safe and easy way to serialize data,
	/// in order to send it over the network using sockets (acid::TcpSocket, acid::UdpSocket).
	///
	/// Packets solve 2 fundamental problems that arise when transferring data over the network:
	/// \li data is interpreted correctly according to the endianness
	/// \li the bounds of the packet are preserved (one send == one receive)
	///
	/// The acid::Packet class provides both input and output modes. It is designed to follow the
	/// behavior of standard C++ streams, using operators >> and << to extract and insert data.
	///
	/// It is recommended to use only fixed-size types (like acid::Int32, etc.),
	/// to avoid possible differences between the sender and the receiver.
	/// Indeed, the native C++ types may have different sizes on two platforms and your data may be
	/// corrupted if that happens.
	/// </summary>
	class ACID_EXPORT Packet
	{
	public:
		/// A bool-like type that cannot be converted to integer or pointer types.
		typedef bool (Packet::*BoolType)(const std::size_t &);

		/// <summary>
		/// Default constructor, creates an empty packet.
		/// </summary>
		Packet();

		virtual ~Packet() = default;

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

		Packet &operator>>(bool &data);

		Packet &operator>>(int8_t &data);

		Packet &operator>>(uint8_t &data);

		Packet &operator>>(int16_t &data);

		Packet &operator>>(uint16_t &data);

		Packet &operator>>(int32_t &data);

		Packet &operator>>(uint32_t &data);

		Packet &operator>>(int64_t &data);

		Packet &operator>>(uint64_t &data);

		Packet &operator>>(float &data);

		Packet &operator>>(double &data);

		Packet &operator>>(char *data);

		Packet &operator>>(std::string &data);

		Packet &operator>>(wchar_t *data);

		Packet &operator>>(std::wstring &data);

		// Overload of operator << to write data into the data stream

		Packet &operator<<(const bool &data);

		Packet &operator<<(const int8_t &data);

		Packet &operator<<(const uint8_t &data);

		Packet &operator<<(const int16_t &data);

		Packet &operator<<(const uint16_t &data);

		Packet &operator<<(const int32_t &data);

		Packet &operator<<(const uint32_t &data);

		Packet &operator<<(const int64_t &data);

		Packet &operator<<(const uint64_t &data);

		Packet &operator<<(const float &data);

		Packet &operator<<(const double &data);

		Packet &operator<<(const char *data);

		Packet &operator<<(const std::string &data);

		Packet &operator<<(const wchar_t *data);

		Packet &operator<<(const std::wstring &data);
	protected:
		friend class TcpSocket;
		friend class UdpSocket;

		/// <summary>
		/// Called before the packet is sent over the network.
		/// This function can be defined by derived classes to transform the data before it is sent;
		/// this can be used for compression, encryption, etc.
		/// The function must return a pointer to the modified data, as well as the number of bytes pointed.
		/// The default implementation provides the packet's data without transforming it.
		/// </summary>
		/// <returns> Pointer to the array of bytes to send and the size of data to send. </returns>
		virtual std::pair<const void *, std::size_t> OnSend();

		/// <summary>
		/// Called after the packet is received over the network.
		/// This function can be defined by derived classes to transform the data after it is received;
		/// this can be used for decompression, decryption, etc.
		/// The function receives a pointer to the received data, and must fill the packet with the transformed bytes.
		/// The default implementation fills the packet directly without transforming the data.
		/// </summary>
		/// <param name="data"> Pointer to the received bytes. </param>
		/// <param name="size"> Number of bytes. </param>
		virtual void OnReceive(const void *data, const std::size_t &size);

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
