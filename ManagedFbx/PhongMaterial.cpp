#include "stdafx.h"
#include "Material.h"

namespace ManagedFbx{
	PhongMaterial::PhongMaterial(FbxSurfacePhong* material) : LambertMaterial(material)
	{
		m_material = material;
	}

	PhongMaterial::PhongMaterial(Material^ material) : LambertMaterial(material)
	{
		m_material = material->m_material;
	}


	string^ PhongMaterial::Diffuse::get()
	{
		return gcnew string(m_material->sDiffuse);
	}

	PhongMaterial::operator PhongMaterial ^ (Material^ material)
	{
		return gcnew PhongMaterial((FbxSurfacePhong*)material->m_material);
	}

	FbxSurfacePhong* PhongMaterial::NativePhongMaterial::get()
	{
		return(FbxSurfacePhong*)m_material;
	}

	//Vector3^ PhongMaterial::AmbientColor::get()
	//{
	//	FbxPropertyT<FbxDouble3> ambientColor = m_material->Ambient;
	//	Vector3^ lColor = gcnew Vector3(ambientColor.Get()[0], ambientColor.Get()[1], ambientColor.Get()[2]);
	//	return lColor;
	//}

	//void PhongMaterial::AmbientColor::set(Vector3^ color)
	//{
	//	FbxPropertyT<FbxDouble3> ambientColor = m_material->Ambient;
	//	//to be continued
	//}
}
