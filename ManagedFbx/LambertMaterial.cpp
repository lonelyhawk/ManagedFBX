#include "stdafx.h"
#include "LambertMaterial.h"


namespace ManagedFbx{
	LambertMaterial::LambertMaterial(FbxSurfaceLambert *material) :AbstractMaterial(material)
	{
		m_material = material;
	}

	LambertMaterial::LambertMaterial(LambertMaterial^ material) : AbstractMaterial(material->m_material)
	{
		m_material = material->m_material;
	}

	//Vector3^ LambertMaterial::AmbientColor::get()
	//{
	//	FbxPropertyT<FbxDouble3> ambientColor = m_material->Ambient;
	//	Vector3^ lColor = gcnew Vector3(ambientColor.Get()[0], ambientColor.Get()[1], ambientColor.Get()[2]);
	//	return lColor;
	//}
	//void LambertMaterial::AmbientColor::set(Vector3^ color)
	//{
	//	FbxPropertyT<FbxDouble3> ambientColor = m_material->Ambient;
	//	//to be continued
	//}

	//Vector3^ LambertMaterial::EmissiveColor::get()
	//{
	//	FbxPropertyT<FbxDouble3> emissiveColor = m_material->Emissive;
	//	Vector3^ lColor = gcnew Vector3(emissiveColor.Get()[0], emissiveColor.Get()[1], emissiveColor.Get()[2]);
	//	return lColor;
	//}
	//void LambertMaterial::EmissiveColor::set(Vector3^ color)
	//{

	//}

	//Vector3^ LambertMaterial::BumpColor::get()
	//{
	//	FbxPropertyT<FbxDouble3> bumpColor = m_material->Bump;
	//	Vector3^ lColor = gcnew Vector3(bumpColor.Get()[0], bumpColor.Get()[1], bumpColor.Get()[2]);
	//	return lColor;
	//}

	//void LambertMaterial::BumpColor::set(Vector3^ color)
	//{

	//}

}
