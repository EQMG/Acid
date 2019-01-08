#pragma once

#include "Serialized/DataStream.hpp"

namespace acid
{
	/// <summary>
	/// Utility class to build blocks of data to transfer over the network.
	/// </summary>
	class ACID_EXPORT Packet :
		public DataStream
	{
	public:
		/// <summary>
		/// Default constructor, creates an empty packet.
		/// </summary>
		Packet();

		~Packet() = default;
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
	};
}
