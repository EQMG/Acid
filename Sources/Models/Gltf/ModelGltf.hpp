#pragma once

#include "Models/Model.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
/// <summary>
/// Class
/// that
/// represents
/// a
/// loaded
/// GLTF
/// model.
/// </summary>
class ACID_EXPORT ModelGltf : public Model
{
  public:
	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// GLTF
	/// model
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// OBJ
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
	static std::shared_ptr<ModelGltf> Create(const Metadata& metadata);

	/// <summary>
	/// Will
	/// find
	/// an
	/// existing
	/// GLTF
	/// model
	/// with
	/// the
	/// same
	/// values,
	/// or
	/// create
	/// a
	/// new
	/// OBJ
	/// model.
	/// </summary>
	/// <param
	/// name="filename">
	/// The
	/// file
	/// to
	/// load
	/// the
	/// OBJ
	/// model
	/// from.
	/// </param>
	static std::shared_ptr<ModelGltf> Create(const std::string& filename);

	/// <summary>
	/// Creates
	/// a
	/// new
	/// GLTF
	/// model.
	/// </summary>
	/// <param
	/// name="filename">
	/// The
	/// file
	/// to
	/// load
	/// the
	/// model
	/// from.
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
	explicit ModelGltf(std::string filename, const bool& load = true);

	void Load() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

  private:
	//	struct
	// Node;
	//	struct
	// Skin;

	std::string m_filename;

	//	std::vector<Node
	//*>
	// nodes;
	//	std::vector<Node
	//*>
	// linearNodes;
	//	std::vector<Skin
	//*>
	// m_skins;
};
}
