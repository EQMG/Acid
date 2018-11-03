#pragma once

#include <istream>
#include <ostream>
#include <string>
#include "Engine/Exports.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	/// <summary>
	/// Encapsulate an IPv4 network address.
	/// </summary>
	class ACID_EXPORT IpAddress
	{
	private:
		/// Address stored as an unsigned 32 bits integer.
		uint32_t m_address;
		/// Is the address valid?
		bool m_valid;
	public:
		/// Value representing an empty/invalid address.
		static const IpAddress NONE;
		/// Value representing any address (0.0.0.0).
		static const IpAddress ANY;
		/// The "localhost" address (for connecting a computer to itself locally).
		static const IpAddress LOCAL_HOST;
		/// The "broadcast" address (for sending UDP messages to everyone on a local network).
		static const IpAddress BROADCAST;

		/// <summary>
		/// Default constructor, this constructor creates an empty (invalid) address.
		/// </summary>
		IpAddress();

		/// <summary>
		/// Construct the address from a string.
		/// Here \a address can be either a decimal address (ex: "192.168.1.56") or a network name (ex: "localhost").
		/// </summary>
		/// <param name="address"> IP address or network name. </param>
		explicit IpAddress(const std::string &address);

		/// <summary>
		/// Construct the address from 4 bytes.
		/// Calling IpAddress(a, b, c, d) is equivalent to calling IpAddress("a.b.c.d"),
		/// but safer as it doesn't have to parse a string to get the address components.
		/// </summary>
		/// <param name="byte0"> First byte of the address. </param>
		/// <param name="byte1"> Second byte of the address. </param>
		/// <param name="byte2"> Third byte of the address. </param>
		/// <param name="byte3"> Fourth byte of the address. </param>
		IpAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);

		/// <summary>
		/// Construct the address from a 32-bits integer.
		///
		/// This constructor uses the internal representation of the address directly.
		/// It should be used for optimization purposes, and only if you got that representation from IpAddress::toInteger().
		/// </summary>
		/// <param name="address"> 4 bytes of the address packed into a 32-bits integer. </param>
		explicit IpAddress(uint32_t address);

		/// <summary>
		/// Get a string representation of the address.
		/// The returned string is the decimal representation of the IP address (like "192.168.1.56"), even if it was constructed from a host name.
		/// </summary>
		/// <returns> String representation of the address. </returns>
		std::string ToString() const;

		/// <summary>
		/// Get an integer representation of the address
		///
		/// The returned number is the internal representation of the
		/// address, and should be used for optimization purposes only
		/// (like sending the address through a socket).
		/// The integer produced by this function can then be converted
		/// back to a IpAddress with the proper constructor.
		/// </summary>
		/// <returns> 32-bits unsigned integer representation of the address. </returns>
		uint32_t ToInteger() const;

		/// <summary>
		/// Get the computer's local address.
		/// The local address is the address of the computer from the LAN point of view,
		/// i.e. something like 192.168.1.56. It is meaningful only for communications over the local network.
		/// Unlike getPublicAddress, this function is fast and may be used safely anywhere.
		/// </summary>
		/// <returns> Local IP address of the computer. </returns>
		static IpAddress GetLocalAddress();

		/// <summary>
		/// Get the computer's public address
		///
		/// The public address is the address of the computer from the internet point of view, i.e. something like 89.54.1.169.
		/// It is necessary for communications over the world wide web. The only way to get a public address is to ask it to a
		/// distant website; as a consequence, this function depends on both your network connection and the server, and may be
		/// very slow. You should use it as few as possible. Because this function depends on the network connection and on a distant
		/// server, you may use a time limit if you don't want your program to be possibly stuck waiting in case there is a problem;
		/// this limit is deactivated by default.
		/// </summary>
		/// <param name="timeout"> Maximum time to wait. </param>
		/// <returns> Public IP address of the computer. </returns>
		static IpAddress GetPublicAddress(Time timeout = Time::ZERO);
	private:
		/// <summary>
		/// Resolve the given address string.
		/// </summary>
		/// <param name="address"> Address string. </param>
		void Resolve(const std::string &address);
	public:
		bool operator==(const IpAddress &other) const;

		bool operator!=(const IpAddress &other) const;

		bool operator<(const IpAddress &other) const;

		bool operator<=(const IpAddress &other) const;

		bool operator>(const IpAddress &other) const;

		bool operator>=(const IpAddress &other) const;

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const IpAddress &address);

		ACID_EXPORT friend std::istream &operator>>(std::istream &stream, IpAddress &address);
	};
}
