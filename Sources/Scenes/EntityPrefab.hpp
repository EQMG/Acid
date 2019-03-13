#pragma once

#include "Files/File.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
class Entity;

/// <summary>
/// Class
/// that
/// represents
/// a
/// entity
/// prefab.
/// </summary>
class ACID_EXPORT EntityPrefab : public Resource
{
  public:
	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// entity
	/// prefab
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// entity
	/// prefab.
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
	static std::shared_ptr<EntityPrefab> Create(const Metadata& metadata);

	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// entity
	/// prefab
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// entity
	/// prefab.
	/// </summary>
	/// <param
	/// name="filename">
	/// The
	/// file
	/// name.
	/// </param>
	static std::shared_ptr<EntityPrefab> Create(const std::string& filename);

	/// <summary>
	/// Creates
	/// a
	/// new
	/// entity
	/// prefab.
	/// </summary>
	/// <param
	/// name="filename">
	/// The
	/// file
	/// name.
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
	explicit EntityPrefab(std::string filename, const bool& load = true);

	void Load() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	void Write(const Entity& entity);

	void Save();

	const std::string& GetFilename() const
	{
		return m_filename;
	}

	Metadata* GetParent() const
	{
		return m_file->GetMetadata();
	}

  private:
	std::string m_filename;
	std::unique_ptr<File> m_file;
};
}
