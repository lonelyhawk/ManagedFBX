#pragma once

#include <msclr\marshal.h>

using namespace msclr::interop;

ref class StringHelper
{
public:
	static const char *ToNative(System::String ^string)
	{
		array<byte, 1>^ bytes = System::Text::Encoding::UTF8->GetBytes(string);
		
		const int lenght = bytes->Length + 1;
		char* chars = new char[lenght];
		for (int i = 0; i < bytes->Length; i++)
		{
			chars[i] = bytes[i];
		}
		chars[lenght - 1] = '\0';
		const char* result = chars;
		return result;
	}

	static const char* FromUTFBytes(array<byte>^ bytes)
	{
		const int length = bytes->Length + 1;
		char* parsedString= new char[length];
		for (int i = 0; i < length - 1; i++)
		{
			parsedString[i] = bytes[i];
		}
		parsedString[length - 1] = '\0';
		const char* result = parsedString;
		return result;
	}

	static array<byte>^ ToUTFBytes(const char* string)
	{
		int length = (int)std::strlen(string);
		array<byte>^ chars = gcnew array <byte>(length);
		for (int i = 0; i < length; i++)
			chars[i] = string[i];
		return chars;
	}

	static System::String^ ToManaged(const char* string)
	{
		const int length = (int)strlen(string);
		array<byte>^ bytes = gcnew array < byte >(length);
		for (int i = 0; i < length; i++)
		{
			bytes[i] = string[i];
		}
		return System::Text::Encoding::UTF8->GetString(bytes);
	}

private:
	static marshal_context ^m_context;
};