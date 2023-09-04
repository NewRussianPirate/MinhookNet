#pragma once
#include <MinHook.h>
#include "MinhookException.h"//Just for correct xml-doc generation :)

namespace MinhookNet
{
	namespace Native
	{
		using namespace System;
		using namespace System::Runtime::InteropServices;
		using namespace System::Runtime::CompilerServices;

		/// <summary>
		/// Wrapped native MH functions. Doesn't throw any <see cref="MinhookNet::Exceptions::MinhookException"/>
		/// </summary>
		public ref class MH_Native abstract sealed
		{
		public:

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="pTarget">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="pDetour">A pointer to the detour function, which will override the target function.</param>
			/// <param name="ppOriginal">A pointer to the trampoline function, which will be used to call the original target function. This parameter can be NULL.</param>
			/// <returns></returns>
			static MHStatus MHCreateHook(IntPtr pTarget, IntPtr pDetour, [Out] IntPtr% ppOriginal);

			/// <summary>
			/// Removes an already created hook.
			/// </summary>
			/// <param name="target">A pointer to the target function.</param>
			/// <returns></returns>
			[MethodImpl(MethodImplOptions::AggressiveInlining)]
			static MHStatus MHRemoveHook(IntPtr target) { return (MHStatus)MH_RemoveHook(target.ToPointer()); }

			/// <summary>
			/// Enables an already created hook.
			/// </summary>
			/// <param name="target">A pointer to the target function. If this parameter is MH_ALL_HOOKS, all created hooks are enabled in one go.</param>
			/// <returns></returns>
			[MethodImpl(MethodImplOptions::AggressiveInlining)]
			static MHStatus MHEnableHook(IntPtr target) { return (MHStatus)MH_EnableHook(target.ToPointer()); }

			/// <summary>
			/// Disabled an already created hook.
			/// </summary>
			/// <param name="target">A pointer to the target function. If this parameter is MH_ALL_HOOKS, all created hooks are disabled in one go.</param>
			/// <returns></returns>
			[MethodImpl(MethodImplOptions::AggressiveInlining)]
			static MHStatus MHDisableHook(IntPtr target) { return (MHStatus)MH_DisableHook(target.ToPointer()); }

			/// <summary>
			/// Creates a hook for the specified API function, in disabled state.
			/// </summary>
			/// <param name="pszModule">A pointer to the loaded module name which contains the target function.</param>
			/// <param name="pszProcName">A pointer to the target function name, which will be overridden by the detour function.</param>
			/// <param name="pDetour">A pointer to the detour function, which will override the target function.</param>
			/// <param name="ppOriginal">A pointer to the trampoline function, which will be used to call the original target function. This parameter can be NULL.</param>
			/// <returns></returns>
			static MHStatus MHCreateHookApi(String^ pszModule, String^ pszProcName, IntPtr pDetour, [Out] IntPtr% ppOriginal);

			/// <summary>
			/// Creates a hook for the specified API function, in disabled state.
			/// </summary>
			/// <param name="pszModule">A pointer to the loaded module name which contains the target function.</param>
			/// <param name="pszProcName">A pointer to the target function name, which will be overridden by the detour function.</param>
			/// <param name="pDetour">A pointer to the detour function, which will override the target function.</param>
			/// <param name="ppOriginal">A pointer to the trampoline function, which will be used to call the original target function. This parameter can be NULL.</param>
			/// <param name="ppTarget">A pointer to the target function, which will be used with other functions. This parameter can be NULL.</param>
			/// <returns></returns>
			static MHStatus MHCreateHookApiEx(String^ pszModule, String^ pszProcName, IntPtr pDetour, [Out] IntPtr% ppOriginal, [Out] IntPtr% ppTarget);

			/// <summary>
			/// Queues to enable an already created hook.
			/// </summary>
			/// <param name="pTarget">A pointer to the target function. If this parameter is MH_ALL_HOOKS, all created hooks are queued to be enabled.</param>
			/// <returns></returns>
			[MethodImpl(MethodImplOptions::AggressiveInlining)]
			static MHStatus MHQueueEnableHook(IntPtr pTarget) { return (MHStatus)MH_QueueEnableHook(pTarget.ToPointer()); }

			/// <summary>
			/// Queues to disable an already created hook.
			/// </summary>
			/// <param name="pTarget">A pointer to the target function. If this parameter is MH_ALL_HOOKS, all created hooks are queued to be disable.</param>
			/// <returns></returns>
			[MethodImpl(MethodImplOptions::AggressiveInlining)]
			static MHStatus MHQueueDisableHook(IntPtr pTarget) { return (MHStatus)MH_QueueDisableHook(pTarget.ToPointer()); }

			/// <summary>
			/// Applies all queued changes in one go.
			/// </summary>
			/// <returns></returns>
			[MethodImpl(MethodImplOptions::AggressiveInlining)]
			static MHStatus MHApplyQueued() { return (MHStatus)MH_ApplyQueued(); }
		};
	}
}