#include "stdafx.h"
#include "Material.h"

namespace ManagedFbx{
	LambertMaterial::LambertMaterial(FbxSurfaceLambert *material) 
		: Material(material)
	{
		m_material = material;
	}

	LambertMaterial::LambertMaterial(LambertMaterial^ material)
		: Material(material->m_material)
	{
		m_material = material->m_material;
	}

	FbxSurfaceLambert* LambertMaterial::NativeLambertMaterial::get()
	{
		return (FbxSurfaceLambert*)m_material;
	}

	Vector3^ LambertMaterial::AmbientColor::get()
	{
		FbxPropertyT<FbxDouble3> ambientColor = NativeLambertMaterial->Ambient;
		Vector3^ lColor = gcnew Vector3(ambientColor.Get()[0], ambientColor.Get()[1], ambientColor.Get()[2]);
		return lColor;
	}
	void LambertMaterial::AmbientColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> ambientColor = NativeLambertMaterial->Ambient;

		FbxDouble3 lColor(color->X, color->Y, color->Z);
		ambientColor.Set(lColor);
		//to be continued
	}

	Vector3^ LambertMaterial::EmissiveColor::get()
	{
		FbxPropertyT<FbxDouble3> emissiveColor = NativeLambertMaterial->Emissive;
		Vector3^ lColor = gcnew Vector3(emissiveColor.Get()[0], emissiveColor.Get()[1], emissiveColor.Get()[2]);
		return lColor;
	}
	void LambertMaterial::EmissiveColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> emissiveColor = NativeLambertMaterial->Emissive;
		FbxDouble3 lColor(color->X, color->Y, color->Z);
		emissiveColor.Set(lColor);
	}

	Vector3^ LambertMaterial::BumpColor::get()
	{
		FbxPropertyT<FbxDouble3> bumpColor = NativeLambertMaterial->Bump;
		Vector3^ lColor = gcnew Vector3(bumpColor.Get()[0], bumpColor.Get()[1], bumpColor.Get()[2]);
		return lColor;
	}

	void LambertMaterial::BumpColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> bumpColor = NativeLambertMaterial->Emissive;
		FbxDouble3 lColor(color->X, color->Y, color->Z);
		bumpColor.Set(lColor);
	}

	void LambertMaterial::DiffuseColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> diffuseColor = NativeLambertMaterial->Diffuse;
		FbxDouble3 lColor(color->X, color->Y, color->Z);
		diffuseColor.Set(lColor);
	}

	Vector3^ LambertMaterial::DiffuseColor::get()
	{
		FbxPropertyT<FbxDouble3> diffuseColor = NativeLambertMaterial->Diffuse;
		Vector3^ lColor = gcnew Vector3(diffuseColor.Get()[0], diffuseColor.Get()[1], diffuseColor.Get()[2]);
		return lColor;
	}


	void LambertMaterial::ConnectDiffuseTexture(ManagedTexture^ texture)
	{
		NativeLambertMaterial->Diffuse.ConnectSrcObject(texture->m_nativeTexture);
	}
	LambertMaterial::operator LambertMaterial ^ (Material^ material)
	{
		return gcnew LambertMaterial((FbxSurfaceLambert*)material->m_material);
	}
}
