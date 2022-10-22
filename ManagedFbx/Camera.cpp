#include "stdafx.h"
#include "Camera.h"

namespace ManagedFbx
{

	Camera::Camera(FbxCamera* nativeCamera)
	{
		m_nativeCamera = nativeCamera;
	}

	ProjectionType Camera::Type::get()
	{
		return (ProjectionType)m_nativeCamera->ProjectionType.Get();
	}

}