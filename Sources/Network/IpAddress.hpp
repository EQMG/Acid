#pragma once

#include <istream>
#include <ostream>
#include <string>
#include "Engine/Exports.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	///
	/// \brief Encapsulate an IPv4 network address
	///
	class ACID_EXPORT IpAddress
	{
	private:
		uint32_t m_address; /// Address stored as an unsigned 32 bits integer
		bool m_valid; /// Is the address valid?
	public:
		static const IpAddress NONE; /// Value representing an empty/invalid address.
		static const IpAddress ANY; /// Value representing any address (0.0.0.0).
		static const IpAddress LOCAL_HOST; /// The "localhost" address (for connecting a computer to itself locally).
		static const IpAddress BROADCAST; /// The "broadcast" address (for sending UDP messages to everyone on a local network).

		///
		/// \brief Default constructor
		///
		/// This constructor creates an empty (invalid) address
		///
		IpAddress();

		///
		/// \brief Construct the address from a string
		///
		/// Here \a address can be either a decimal address
		/// (ex: "192.168.1.56") or a network name (ex: "localhost").
		///
		/// \param address IP address or network name
		///
		IpAddress(const std::string &address);

		///
		/// \brief Construct the address from 4 bytes
		///
		/// Calling IpAddress(a, b, c, d) is equivalent to calling
		/// IpAddress("a.b.c.d"), but safer as it doesn't have to
		/// parse a string to get the address components.
		///
		/// \param byte0 First byte of the address
		/// \param byte1 Second byte of the address
		/// \param byte2 Third byte of the address
		/// \param byte3 Fourth byte of the address
		///
		IpAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);

		///
		/// \brief Construct the address from a 32-bits integer
		///
		/// This constructor uses the internal representation of
		/// the address directly. It should be used for optimization
		/// purposes, and only if you got that representation from
		/// IpAddress::toInteger().
		///
		/// \param address 4 bytes of the address packed into a 32-bits integer
		///
		/// \see toInteger
		///
		explicit IpAddress(uint32_t address);

		///
		/// \brief Get a string representation of the address
		///
		/// The returned string is the decimal representation of the
		/// IP address (like "192.168.1.56"), even if it was constructed
		/// from a host name.
		///
		/// \return String representation of the address
		///
		/// \see toInteger
		///
		std::string ToString() const;

		///
		/// \brief Get an integer representation of the address
		///
		/// The returned number is the internal representation of the
		/// address, and should be used for optimization purposes only
		/// (like sending the address through a socket).
		/// The integer produced by this function can then be converted
		/// back to a IpAddress with the proper constructor.
		///
		/// \return 32-bits unsigned integer representation of the address
		///
		/// \see toString
		///
		uint32_t ToInteger() const;

		///
		/// \brief Get the computer's local address
		///
		/// The local address is the address of the computer from the
		/// LAN point of view, i.e. something like 192.168.1.56. It is
		/// meaningful only for communications over the local network.
		/// Unlike getPublicAddress, this function is fast and may be
		/// used safely anywhere.
		///
		/// \return Local IP address of the computer
		///
		/// \see getPublicAddress
		///
		static IpAddress GetLocalAddress();

		///
		/// \brief Get the computer's public address
		///
		/// The public address is the address of the computer from the
		/// internet point of view, i.e. something like 89.54.1.169.
		/// It is necessary for communications over the world wide web.
		/// The only way to get a public address is to ask it to a
		/// distant website; as a consequence, this function depends on
		/// both your network connection and the server, and may be
		/// very slow. You should use it as few as possible. Because
		/// this function depends on the network connection and on a distant
		/// server, you may use a time limit if you don't want your program
		/// to be possibly stuck waiting in case there is a problem; this
		/// limit is deactivated by default.
		///
		/// \param timeout Maximum time to wait
		///
		/// \return Public IP address of the computer
		///
		/// \see getLocalAddress
		///
		//	static IpAddress GetPublicAddress(Time timeout = Time::Zero);
	private:
		///
		/// \brief Resolve the given address string
		///
		/// \param address Address string
		///
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
