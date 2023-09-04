#pragma once
#include "MinhookException.h"
#include <MinHook.h>

namespace MinhookNet
{
	using namespace System;
	using namespace System::Runtime::InteropServices;

	namespace Hooks
	{
		using namespace System::Runtime::CompilerServices;

		public ref class HookBase abstract
		{
		private:
			IntPtr _trampolineAddr;
			IntPtr _targetAddr;

			bool _isEnabled = false;
			bool _created = false;

		protected:

			IntPtr _detourAddr;

			/// <summary>
			/// Check <see cref="Minhook::Initialized"/> and call <see cref="Minhook::Initialize()"/> if false.
			/// </summary>
			/// <exception cref="MinhookNet::Exceptions::MinhookException"></exception>
			HookBase()
			{
				if (Minhook::Initialized)
					return;
				Minhook::Initialize();
			}

		protected:

			bool _isDisposed = false;

			/// <summary>
			/// This is basically 'MH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal)' with "_targetAddr", "_detourAddr", "_trampolineAddr" as args.
			/// Hook.Hook(IntPtr, IntPtr) calls this by default. 
			/// </summary>
			/// <exception cref="System::ObjectDisposedException">Hook has been disposed.</exception>
			/// <exception cref="MinhookNet::Exceptions::MinhookException">MH error.</exception>
			void CreateHook()
			{
				if (_created)
					return;
				if (_targetAddr == IntPtr::Zero || _detourAddr == IntPtr::Zero)
					throw gcnew ArgumentException("Pointers are null.");
				if (_isDisposed)
					throw gcnew ObjectDisposedException(HookBase::GetType()->Name, "Hook has been disposed.");
				void* trampoline;
				if (auto status = MH_CreateHook((LPVOID)(_targetAddr.ToPointer()), (LPVOID)(_detourAddr.ToPointer()), &trampoline); status != MH_STATUS::MH_OK)
					throw gcnew MinhookNet::Exceptions::MinhookException("Failed to create hook.", (MHStatus)status, _targetAddr, _detourAddr, _trampolineAddr);
				_trampolineAddr = IntPtr(trampoline);
				_created = true;
			}

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">Target function/method for hook</param>
			/// <param name="detour"></param>
			/// <exception cref="MinhookNet::Exceptions::MinhookException">MH error.</exception>
			HookBase(IntPtr target, IntPtr detour);

			HookBase(long long target, long long detour) : HookBase(IntPtr(target), IntPtr(detour)) {}

		public:
			/// <summary>
			/// Disable and remove hook.
			/// </summary>
			virtual ~HookBase();
			
			!HookBase();

			property bool IsEnabled
			{
				bool get() { return _isEnabled; }
			}

			/// <summary>
			/// Get target as <see cref="System::IntPtr"/>.
			/// </summary>
			property IntPtr TargetAddress
			{
				IntPtr get() { return _targetAddr; }
			}

			/// <summary>
			/// Get detour as <see cref="System::IntPtr"/>.
			/// </summary>
			property IntPtr DetourAddress
			{
				IntPtr get() { return _detourAddr; }
			}

			/// <summary>
			/// Get trampoline as <see cref="System::IntPtr"/>.
			/// </summary>
			property IntPtr TrampolineAddress
			{
				IntPtr get() { return _trampolineAddr; }
			}

			/// <summary>
			/// Sets target function without creating MH hook itself.
			/// </summary>
			/// <param name="target"></param>
			HookBase(IntPtr target) : HookBase()
			{
				if (target == IntPtr::Zero)
					throw gcnew ArgumentNullException();
				_targetAddr = target;
			}

			HookBase(long long target) : HookBase(IntPtr(target)) {}

			/// <summary>
			/// Enable created hook. Returns false if hook was already enabled.
			/// </summary>
			/// <returns></returns>
			/// <exception cref="System::ObjectDisposedException">Hook has been disposed.</exception>
			/// <exception cref="System::NullReferenceException">Hook was not created.</exception>
			/// <exception cref="System::NullReferenceException">_targetAddr is nullptr.</exception>
			/// <exception cref="MinhookNet::Exceptions::MinhookException">MH error.</exception>
			bool Enable();

			/// <summary>
			/// Disable created hook. Returns false if hook was already disabled.
			/// </summary>
			/// <returns></returns>
			/// <exception cref="System::ObjectDisposedException">Hook has been disposed.</exception>
			/// <exception cref="System::NullReferenceException">Hook was not created.</exception>
			/// <exception cref="System::NullReferenceException">_targetAddr is nullptr.</exception>
			/// <exception cref="MinhookNet::Exceptions::MinhookException">MH error.</exception>
			bool Disable();
		};


		public ref class Hook : HookBase
		{
		public:

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">A pointer to the detour function, which will override the target function.</param>
			Hook(IntPtr target, IntPtr detour) : HookBase(target, detour) {}

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">An address of the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">An address of the detour function, which will override the target function.</param>
			Hook(long long target, long long detour) : HookBase(target, detour) {}
		};
	}
}
