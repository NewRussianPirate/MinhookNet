#include "FunctionHook.h"

generic<typename TDelegate>
MinhookNet::Hooks::FunctionHook<TDelegate>::FunctionHook(IntPtr target, IntPtr detour) : Hook(target, detour)
{
	_target = Marshal::GetDelegateForFunctionPointer<TDelegate>(target);
	_detour = Marshal::GetDelegateForFunctionPointer<TDelegate>(detour);
	_trampoline = Marshal::GetDelegateForFunctionPointer<TDelegate>(TrampolineAddress);
}

generic<typename TDelegate>
MinhookNet::Hooks::FunctionHook<TDelegate>::FunctionHook(IntPtr target, TDelegate detour) : Hook(target, Marshal::GetFunctionPointerForDelegate(detour))
{
	_detour = detour;
	_target = Marshal::GetDelegateForFunctionPointer<TDelegate>(target);
	_trampoline = Marshal::GetDelegateForFunctionPointer<TDelegate>(TrampolineAddress);
}

generic<typename TDelegate>
MinhookNet::Hooks::FunctionHook<TDelegate>::FunctionHook(TDelegate target, TDelegate detour) : Hook(Marshal::GetFunctionPointerForDelegate(target), Marshal::GetFunctionPointerForDelegate(detour))
{
	_target = target;
	_detour = detour;
	_trampoline = Marshal::GetDelegateForFunctionPointer<TDelegate>(TrampolineAddress);
}

generic<typename TDelegate>
MinhookNet::Hooks::FunctionHook<TDelegate>::FunctionHook(TDelegate target, IntPtr detour) : Hook(Marshal::GetFunctionPointerForDelegate(target), detour)
{
	_target = target;
	_detour = Marshal::GetDelegateForFunctionPointer<TDelegate>(detour);
	_trampoline = Marshal::GetDelegateForFunctionPointer<TDelegate>(TrampolineAddress);
}
