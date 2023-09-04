#pragma once
#include <vcclr.h>

using namespace System::Runtime::InteropServices;

namespace StringHelper
{
	using namespace System;
	struct C8Holder
	{
	private:
		const char* _str;
		bool _deleted = false;

	public:

		C8Holder(String^ s) : _str((const char*)(void*)Marshal::StringToHGlobalAnsi(s)) {}

		C8Holder(C8Holder && move) : _str(std::move(move._str)) {}

		~C8Holder() { free(); }

		const char *const str() const noexcept { return _str; }

		void free() 
		{ 
			if (!_deleted)
			{
				Marshal::FreeHGlobal(IntPtr((void*)_str));
				_deleted = true;
			}
		}
	};
	
	//Copy System.String as c8 with HGlobalAnsi
	C8Holder ToC8Global(String^ s) { return C8Holder{ s }; }

	void FreeHGlobalAnsi(C8Holder& c8Str) { c8Str.free(); }
}


