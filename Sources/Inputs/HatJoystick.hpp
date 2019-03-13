#pragma once

#include "Devices/Joysticks.hpp"
#include "IAxis.hpp"
#include "IButton.hpp"

namespace acid
{
/// <summary>
/// Button
/// from a
/// joystick.
/// </summary>
class ACID_EXPORT HatJoystick : public IAxis, public IButton
{
  public:
	/// <summary>
	/// Creates
	/// a
	/// new
	/// joystick
	/// button.
	/// </summary>
	/// <param
	/// name="port">
	/// The
	/// joystick
	/// port.
	/// </param>
	/// <param
	/// name="hat">
	/// The
	/// hat
	/// that
	/// will
	/// be
	/// checked.
	/// </param>
	/// <param
	/// name="hatFlag">
	/// If
	/// this
	/// bit
	/// is
	/// found
	/// the
	/// hat
	/// will
	/// trigger
	/// <seealso
	/// cref="#IsDown()"/>.
	/// </param>
	HatJoystick(const uint32_t& port, const uint32_t& hat, const JoystickHat& hatFlag);

	float GetAmount() const override;

	bool IsDown() const override;

	bool WasDown() override;

	const uint32_t& GetPort() const
	{
		return m_port;
	}

	void SetPort(const uint32_t& port)
	{
		m_port = port;
	}

	const uint32_t& GetHat() const
	{
		return m_hat;
	}

	void SetHat(const uint32_t& hat)
	{
		m_hat = hat;
	}

	const JoystickHat& GetHatFlag() const
	{
		return m_hatFlag;
	}

	void SetHatFlag(const JoystickHat& hatFlag)
	{
		m_hatFlag = hatFlag;
	}

  private:
	uint32_t m_port;
	uint32_t m_hat;
	JoystickHat m_hatFlag;
	bool m_wasDown;
};
}
