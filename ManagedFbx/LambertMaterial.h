#pragma once
#include "AbstractMaterial.h"

namespace ManagedFbx{
	public ref class LambertMaterial : AbstractMaterial
	{
	public:
		LambertMaterial(FbxSurfaceLambert* material);
		LambertMaterial(LambertMaterial^ material);
/*
		property_rw(Vector3^, AmbientColor);
		property_rw(Vector3^, EmissiveColor);
		property_rw(Vector3^, BumpColor)*/

	private:
		FbxSurfaceLambert* m_material;
	};
}

