#pragma once
#include "MinHook.h"

namespace MinhookNet
{
	public ref class Minhook abstract sealed
	{
	private:
		static bool _initialized = false;

	public:

		static property bool Initialized
		{
			bool get() { return _initialized; }
		}

		/// <summary>
		/// Initialize the MinHook library. You must call this function EXACTLY ONCE at the beginning of your program.
		/// This also will be called on Hook's instance creation.
		/// (MH_Initialize())
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Uninitialize the MinHook library. You must call this function EXACTLY ONCE at the end of your program.
		/// (MH_Uninitialize())
		/// </summary>
		static void Uninitialize();
	};
}


