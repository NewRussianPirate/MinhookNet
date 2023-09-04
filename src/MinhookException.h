#pragma once
#include "MinHook.h"
#include "MHStatus.h"

namespace MinhookNet
{
	namespace Exceptions
	{
		using namespace System;
		public ref class MinhookException : public Exception
		{
		private:
			MHStatus _errorStatus;

			IntPtr _target;
			IntPtr _detour;
			IntPtr _trampoline;

		public:
			property MHStatus ErrorStatus
			{
				MHStatus get() { return _errorStatus; }
			}

			property IntPtr Target
			{
				IntPtr get() { return _target; }
			}

			property IntPtr Detour
			{
				IntPtr get() { return _detour; }
			}

			property IntPtr Trampolione
			{
				IntPtr get() { return _trampoline; }
			}

			MinhookException(String^ msg, MHStatus error);
			MinhookException(String^ msg, MHStatus error, IntPtr target, IntPtr detour, IntPtr trampoline);
		};
	}
}


