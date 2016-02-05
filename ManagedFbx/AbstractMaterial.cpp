#include "stdafx.h"
#include "AbstractMaterial.h"
#include <msclr/marshal_cppstd.h>

namespace ManagedFbx{

	AbstractMaterial::AbstractMaterial(FbxSurfaceLambert* material)
	{
		m_material = material;
	}

	Vector3^ AbstractMaterial::AmbientColor::get()
	{
		FbxPropertyT<FbxDouble3> ambientColor = m_material->Ambient;
		Vector3^ lColor = gcnew Vector3(ambientColor.Get()[0], ambientColor.Get()[1], ambientColor.Get()[2]);
		return lColor;
	}
	void AbstractMaterial::AmbientColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> ambientColor = m_material->Ambient;

		FbxDouble3 lColor(color->X, color->Y, color->Z);
		ambientColor.Set(lColor);
		//to be continued
	}

	Vector3^ AbstractMaterial::EmissiveColor::get()
	{
		FbxPropertyT<FbxDouble3> emissiveColor = m_material->Emissive;
		Vector3^ lColor = gcnew Vector3(emissiveColor.Get()[0], emissiveColor.Get()[1], emissiveColor.Get()[2]);
		return lColor;
	}
	void AbstractMaterial::EmissiveColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> emissiveColor = m_material->Emissive;
		FbxDouble3 lColor(color->X, color->Y, color->Z);
		emissiveColor.Set(lColor);
	}

	Vector3^ AbstractMaterial::BumpColor::get()
	{
		FbxPropertyT<FbxDouble3> bumpColor = m_material->Bump;
		Vector3^ lColor = gcnew Vector3(bumpColor.Get()[0], bumpColor.Get()[1], bumpColor.Get()[2]);
		return lColor;
	}

	void AbstractMaterial::BumpColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> bumpColor= m_material->Emissive;
		FbxDouble3 lColor(color->X, color->Y, color->Z);
		bumpColor.Set(lColor);
	}

	void AbstractMaterial::DiffuseColor::set(Vector3^ color)
	{
		FbxPropertyT<FbxDouble3> diffuseColor = m_material->Diffuse;
		FbxDouble3 lColor(color->X, color->Y, color->Z);
		diffuseColor.Set(lColor);
	}

	Vector3^ AbstractMaterial::DiffuseColor::get()
	{
		FbxPropertyT<FbxDouble3> diffuseColor = m_material->Diffuse;
		Vector3^ lColor = gcnew Vector3(diffuseColor.Get()[0], diffuseColor.Get()[1], diffuseColor.Get()[2]);
		return lColor;
	}

	string^ AbstractMaterial::Name::get()
	{
		return gcnew string(m_material->GetName());
	}

	void AbstractMaterial::Name::set(string^ name)
	{
		std::string converted_name = msclr::interop::marshal_as< std::string >(name);
		m_material->SetName(converted_name.c_str());
	}

	void AbstractMaterial::ConnectDiffuseTexture(ManagedTexture^ texture)
	{
		m_material->Diffuse.ConnectSrcObject(texture->m_nativeTexture);
	}
}
