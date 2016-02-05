#pragma once

namespace ManagedFbx{
	public ref class ManagedTexture
	{
	public:

		property_rw(string^, FileName);

	internal:
		ManagedTexture(FbxFileTexture *);
		FbxFileTexture* m_nativeTexture;
	};
}