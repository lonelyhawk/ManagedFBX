#pragma once

#include "SceneObject.h"

namespace ManagedFbx
{
	public enum class LightType
	{
		Point = FbxLight::ePoint,
		Spot = FbxLight::eSpot,
		Directional = FbxLight::eDirectional
	};

	public ref class Light: SceneObject
	{
	public:
		property_r(LightType, Type);
		property_r(Vector3, Colour);
	internal:
		Light(FbxLight *nativeLight);
	private:
		FbxLight *m_nativeLight;
	};
}