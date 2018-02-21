#include "ComponentMaterial.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentMaterial::ComponentMaterial(const float &metallic, const float &roughness, const Colour &diffuse, const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog, Texture *material) :
		Component(),
		m_metallic(metallic),
		m_roughness(roughness),
		m_diffuse(new Colour(diffuse)),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_textureMaterial(material)
	{
	}

	ComponentMaterial::ComponentMaterial(ComponentPrefab *prefab) :
		Component(),
		m_metallic(prefab->GetFloat(0)),
		m_roughness(prefab->GetFloat(1)),
		m_diffuse(new Colour(prefab->GetString(2))),
		m_castsShadows(prefab->GetBool(3)),
		m_ignoreLighting(prefab->GetBool(4)),
		m_ignoreFog(prefab->GetBool(5)),
		m_textureMaterial(prefab->GetString(6) == "nullptr" ? nullptr : Texture::Resource(prefab->GetString(6)))
	{
	}

	ComponentMaterial::~ComponentMaterial()
	{
		delete m_diffuse;
	}

	void ComponentMaterial::Update()
	{
	}

	void ComponentMaterial::CmdRender(EntityRender *entityRender)
	{
		if (m_textureMaterial != nullptr)
		{
			entityRender->descriptorWrites.push_back(m_textureMaterial->GetWriteDescriptor(4, entityRender->descriptorSet));
			entityRender->uboObject.samples.m_z = 1.0f;
		}

		entityRender->uboObject.diffuse = Colour(*m_diffuse);
		entityRender->uboObject.surface = Vector3(m_metallic, m_roughness, (1.0f / 3.0f) * ((float) m_ignoreFog + (2.0f * (float) m_ignoreLighting)));
	}

//	void ComponentMaterial::Save(ComponentPrefab *prefab)
//	{
//		prefab->SetFloat(0, m_metallic);
//		prefab->SetFloat(1, m_roughness);
//		prefab->SetString(2, Colour::GetHex(*m_diffuse));
//		prefab->SetBool(3, m_castsShadows);
//		prefab->SetBool(4, m_ignoreLighting);
//		prefab->SetBool(5, m_ignoreFog);
//		prefab->SetString(6, m_textureMaterial != nullptr ? m_textureMaterial->GetFilename() : "nullptr");
//	}
}
