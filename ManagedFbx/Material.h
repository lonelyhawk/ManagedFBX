#pragma once
#include "LambertMaterial.h"
#include "PhongMaterial.h"

namespace ManagedFbx{
	public ref class Material
	{
	public:
		property_r(enum class ManagedMaterialType, MaterialType);
		static operator LambertMaterial^(Material^ material);
		static operator PhongMaterial^(Material^ material);

	protected:
		FbxSurfaceMaterial* m_material;

	internal:
		Material(FbxSurfaceMaterial* material);
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
}