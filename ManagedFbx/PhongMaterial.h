#pragma once
#include "AbstractMaterial.h"

namespace ManagedFbx{
	public ref class PhongMaterial : AbstractMaterial
	{
	public:
		PhongMaterial(FbxSurfacePhong* material);

		property_r(string^, Diffuse);

	private:
		FbxSurfacePhong * m_material;
	};
}

