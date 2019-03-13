#pragma once

#include "AcidPch.hpp"

namespace acid
{
class Metadata;

/// <summary>
/// A
/// managed
/// resource
/// object.
/// Implementations
/// contain
/// Create
/// functions
/// that
/// can
/// take a
/// metadata
/// object
/// or
/// pass
/// parameters
/// to the
/// constructor.
/// </summary>
class ACID_EXPORT Resource
{
  public:
	Resource() = default;

	virtual ~Resource() = default;

	/// <summary>
	/// Used
	/// by
	/// the
	/// resource
	/// after
	/// it
	/// has
	/// been
	/// decoded,
	/// and
	/// in
	/// constructors.
	/// </summary>
	virtual void Load() {}

	/// <summary>
	/// Used
	/// to
	/// decode
	/// this
	/// resource
	/// from
	/// a
	/// loaded
	/// data
	/// format.
	/// </summary>
	/// <param
	/// name="metadata">
	/// The
	/// metadata
	/// to
	/// decode
	/// from.
	/// </param>
	virtual void Decode(const Metadata& metadata) {}

	/// <summary>
	/// Used
	/// to
	/// encode
	/// this
	/// resource
	/// into
	/// a
	/// data
	/// format.
	/// </summary>
	/// <param
	/// name="metadata">
	/// The
	/// metadata
	/// to
	/// encode
	/// into.
	/// </param>
	virtual void Encode(Metadata& metadata) const {}
};
}
