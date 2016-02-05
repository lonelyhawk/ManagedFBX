#include "stdafx.h"
#include "ManagedTexture.h"
#include <msclr\marshal_cppstd.h>

using namespace ManagedFbx;

ManagedTexture::ManagedTexture(FbxFileTexture* texture)
{
	m_nativeTexture = texture;
}

string^ ManagedTexture::FileName::get()
{
	return gcnew string(m_nativeTexture->GetFileName());
}

void ManagedTexture::FileName::set(string^ value)
{
	std::string converted_name = msclr::interop::marshal_as< std::string >(value);
	m_nativeTexture->SetFileName(converted_name.c_str());
}