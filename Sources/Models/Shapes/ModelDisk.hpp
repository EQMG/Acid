#pragma once

#include "Models/Model.hpp"

namespace acid
{
/// <summary>
/// Class
/// that
/// represents
/// a disk
/// model.
/// </summary>
class ACID_EXPORT ModelDisk : public Model
{
  public:
	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// disk
	/// model
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// disk
	/// model.
	/// </summary>
	/// <param
	/// name="metadata">
	/// The
	/// metadata
	/// to
	/// decode
	/// values
	/// from.
	/// </param>
	static std::shared_ptr<ModelDisk> Create(const Metadata& metadata);

	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// disk
	/// model
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// disk
	/// model.
	/// </summary>
	/// <param
	/// name="innerRadius">
	/// The
	/// inner
	/// radius.
	/// </param>
	/// <param
	/// name="outerRadius">
	/// The
	/// outer
	/// radius.
	/// </param>>
	/// <param
	/// name="slices">
	/// The
	/// amount
	/// of
	/// vertices
	/// that
	/// will
	/// be
	/// used
	/// to
	/// create
	/// the
	/// circular
	/// footprint.
	/// </param>
	/// <param
	/// name="loops">
	/// The
	/// amount
	/// of
	/// circles
	/// that
	/// will
	/// be
	/// created
	/// between
	/// the
	/// inner
	/// and
	/// outer
	/// radius.
	/// </param>
	static std::shared_ptr<ModelDisk> Create(const float& innerRadius = 0.5f, const float& outerRadius = 1.0f, const uint32_t& slices = 20, const uint32_t& loops = 4);

	/// <summary>
	/// Creates
	/// a
	/// new
	/// disk
	/// model.
	/// </summary>
	/// <param
	/// name="innerRadius">
	/// The
	/// inner
	/// radius.
	/// </param>
	/// <param
	/// name="outerRadius">
	/// The
	/// outer
	/// radius.
	/// </param>>
	/// <param
	/// name="slices">
	/// The
	/// amount
	/// of
	/// vertices
	/// that
	/// will
	/// be
	/// used
	/// to
	/// create
	/// the
	/// circular
	/// footprint.
	/// </param>
	/// <param
	/// name="loops">
	/// The
	/// amount
	/// of
	/// circles
	/// that
	/// will
	/// be
	/// created
	/// between
	/// the
	/// inner
	/// and
	/// outer
	/// radius.
	/// </param>
	/// <param
	/// name="load">
	/// If
	/// this
	/// resource
	/// will
	/// load
	/// immediately,
	/// otherwise
	/// <seealso
	/// cref="#Load()"/>
	/// can
	/// be
	/// called.
	/// </param>
	explicit ModelDisk(const float& innerRadius = 0.5f, const float& outerRadius = 1.0f, const uint32_t& slices = 20, const uint32_t& loops = 4, const bool& load = true);

	void Load() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

  private:
	float m_innerRadius;
	float m_outerRadius;
	uint32_t m_slices;
	uint32_t m_loops;
};
}
