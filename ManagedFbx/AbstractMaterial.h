#pragma once
#include "ManagedTexture.h"

namespace ManagedFbx{
	public ref class AbstractMaterial
	{
	public:
		AbstractMaterial(FbxSurfaceLambert* material);

		virtual property_rw(Vector3^, AmbientColor);
		virtual property_rw(Vector3^, EmissiveColor);
		virtual property_rw(Vector3^, BumpColor);
		virtual property_rw(Vector3^, DiffuseColor);
		virtual property_rw(string^, Name);

		virtual void ConnectDiffuseTexture(ManagedTexture^);

	internal:
		FbxSurfaceLambert* m_material;
	};
}

