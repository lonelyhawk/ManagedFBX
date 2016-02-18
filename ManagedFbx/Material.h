#pragma once
#include "ManagedTexture.h"
#include <msclr/marshal_cppstd.h>

namespace ManagedFbx{
	public ref class Material
	{
	public:
		property_r(enum class ManagedMaterialType, MaterialType);
		/*static operator LambertMaterial^(Material^ material);
		static operator PhongMaterial^(Material^ material)*/;
		property_rw(string^, Name);

	protected:

	internal:
		Material(FbxSurfaceMaterial* material);
		FbxSurfaceMaterial* m_material;
	private:
	};

	public enum NativeMaterialType
	{
		LAMBERT, PHONG
	};

	public enum class ManagedMaterialType
	{
		LAMBERT = NativeMaterialType::LAMBERT,
		PHONG = NativeMaterialType::PHONG
	};

	public ref class LambertMaterial : Material
	{
	public:
		LambertMaterial(LambertMaterial^ material);

		virtual property_rw(Vector3^, AmbientColor);
		virtual property_rw(Vector3^, EmissiveColor);
		virtual property_rw(Vector3^, BumpColor);
		virtual property_rw(Vector3^, DiffuseColor);

		virtual void ConnectDiffuseTexture(ManagedTexture^);

		static operator LambertMaterial ^ (Material^);

	internal:
		LambertMaterial(FbxSurfaceLambert* material);
		virtual property_r(FbxSurfaceLambert*, NativeLambertMaterial);
	};


	public ref class PhongMaterial : LambertMaterial
	{
	public:
		PhongMaterial(Material^);

		property_r(string^, Diffuse);
		static operator PhongMaterial ^ (Material^);
	internal:
		PhongMaterial(FbxSurfacePhong* material);
		property_r(FbxSurfacePhong*, NativePhongMaterial);
	};
}