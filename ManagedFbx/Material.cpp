#include "stdafx.h"
#include "Material.h"
#include <msclr/marshal_cppstd.h>

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

	string^ Material::Name::get()
	{
		return gcnew string(m_material->GetName());
	}

	void Material::Name::set(string^ name)
	{
		std::string converted_name = msclr::interop::marshal_as< std::string >(name);
		m_material->SetName(converted_name.c_str());
	}
}


