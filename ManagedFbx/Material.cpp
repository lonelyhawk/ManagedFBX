#include "stdafx.h"
#include "Material.h"


namespace ManagedFbx{
	Material::Material(FbxSurfaceMaterial *material)
	{
		m_material = material;
	}

	enum class ManagedMaterialType Material::MaterialType::get()
	{
		if (m_material->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			return ManagedFbx::ManagedMaterialType::PHONG;
		}
		else
		{
			return ManagedFbx::ManagedMaterialType::LAMBERT;
		}
	}

	Material::operator ManagedFbx::LambertMaterial^(Material ^ material)
	{
		return gcnew LambertMaterial((FbxSurfaceLambert*)material->m_material);
	}

	Material::operator ManagedFbx::PhongMaterial^(Material^ material)
	{
		return gcnew PhongMaterial((FbxSurfacePhong*)material->m_material);
	}

}


