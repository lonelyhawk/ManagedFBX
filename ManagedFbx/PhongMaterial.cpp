#include "stdafx.h"
#include "PhongMaterial.h"

namespace ManagedFbx{
	PhongMaterial::PhongMaterial(FbxSurfacePhong* material) : AbstractMaterial(material)
	{
		m_material = material;
	}

	string^ PhongMaterial::Diffuse::get()
	{
		return gcnew string(m_material->sDiffuse);
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
