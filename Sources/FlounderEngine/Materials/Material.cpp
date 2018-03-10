#include "Material.hpp"

namespace Flounder
{
	Material::Material(MaterialDiffuse *diffuse, MaterialSurface *surface, MaterialNormal *normal, MaterialSway *sway) :
		Component(),
		m_diffuse(diffuse),
		m_surface(surface),
		m_normal(normal),
		m_sway(sway)
	{
	}

	Material::~Material()
	{
		delete m_diffuse;
		delete m_surface;
		delete m_normal;
		delete m_sway;
	}

	void Material::Update()
	{
	}

	void Material::Load(LoadedValue *value)
	{
		m_diffuse->Load(value->GetChild("Diffuse"));
		m_surface->Load(value->GetChild("Surface"));
		m_normal->Load(value->GetChild("Normal"));
		m_sway->Load(value->GetChild("Sway"));
	}

	void Material::Write(LoadedValue *destination)
	{
		m_diffuse->Write(destination->GetChild("Diffuse", true));
		m_surface->Write(destination->GetChild("Surface", true));
		m_normal->Write(destination->GetChild("Normal", true));
		m_sway->Write(destination->GetChild("Sway", true));
	}
}
