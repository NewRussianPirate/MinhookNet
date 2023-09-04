#pragma once
#include "Hook.h"

using namespace System;

namespace MinhookNet
{
	namespace Hooks
	{
		/// <summary>
		/// Hook that represents target, detour and trampoline as <see cref="System::Delegate"/>
		/// </summary>
		/// <typeparam name="TDelegate">Specified <see cref="System::Delegate"/> type.</typeparam>
		generic<typename TDelegate>
		where TDelegate : Delegate
		public ref class FunctionHook : public Hook
		{
		private:
			TDelegate _target;
			TDelegate _detour;
			TDelegate _trampoline;

		public:

			/// <summary>
			/// Get target as <see cref="TDelegate"/>.
			/// </summary>
			property TDelegate Target
			{
				TDelegate get() { return _target; }
			}

			/// <summary>
			/// Get detour as <see cref="TDelegate"/>.
			/// </summary>
			property TDelegate Detour
			{
				TDelegate get() { return _detour; }
			}

			/// <summary>
			/// Get trampoline as <see cref="TDelegate"/>.
			/// </summary>
			property TDelegate Trampoline
			{
				TDelegate get() { return _trampoline; }
			}

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">A pointer to the detour function, which will override the target function.</param>
			FunctionHook(IntPtr target, IntPtr detour);

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">A pointer to the detour function, which will override the target function.</param>
			FunctionHook(IntPtr target, TDelegate detour);

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">A pointer to the detour function, which will override the target function.</param>
			FunctionHook(TDelegate target, TDelegate detour);

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">A pointer to the detour function, which will override the target function.</param>
			FunctionHook(TDelegate target, IntPtr detour);

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">An address of the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">An address of the detour function, which will override the target function.</param>
			FunctionHook(long long target, long long detour) : FunctionHook(IntPtr(target), IntPtr(detour)) {}

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">An address of the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">A pointer to the detour function, which will override the target function.</param>
			FunctionHook(long long target, TDelegate detour) : FunctionHook(IntPtr(target), detour) {}

			/// <summary>
			/// Creates a hook for the specified target function, in disabled state.
			/// </summary>
			/// <param name="target">A pointer to the target function, which will be overridden by the detour function.</param>
			/// <param name="detour">An address of the detour function, which will override the target function.</param>
			FunctionHook(TDelegate target, long long detour) : FunctionHook(target, IntPtr(detour)) {}

			~FunctionHook() override
			{
				if (_isDisposed)
					return;
				_trampoline = _detour = _target = TDelegate();
			}
		};
	}
}

