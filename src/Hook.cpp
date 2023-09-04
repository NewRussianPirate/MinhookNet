#include "Hook.h"

MinhookNet::Hooks::HookBase::HookBase(IntPtr target, IntPtr detour) : HookBase(target)
{
	if (detour == IntPtr::Zero)
		throw gcnew ArgumentNullException("target or detour are null.");

	_detourAddr = detour;
	CreateHook();
}

MinhookNet::Hooks::HookBase::~HookBase()
{
	if (_isDisposed)
		return;
	this->!HookBase();
	_targetAddr = _detourAddr = _trampolineAddr = IntPtr::Zero;
	_isDisposed = true;
}

MinhookNet::Hooks::HookBase::!HookBase()
{
	if (_created)
	{
		Disable();
		if (auto status = MH_RemoveHook(_targetAddr.ToPointer()); status != MH_OK && status != MH_ERROR_NOT_CREATED)
			throw gcnew MinhookNet::Exceptions::MinhookException("Failed to remove hook", (MHStatus)status, _targetAddr, _detourAddr, _trampolineAddr);
	}
	_isDisposed = true;
}

bool MinhookNet::Hooks::HookBase::Enable()
{
	if (_isDisposed)
		throw gcnew ObjectDisposedException(HookBase::GetType()->Name, "Hook has been disposed.");
	if (!_created)
		throw gcnew NullReferenceException("Hook wasn't created.");
	if (_isEnabled)
		return false;
	if (_targetAddr == IntPtr::Zero)
		throw gcnew NullReferenceException("_targetAddr is nullptr.");
	if (auto status = MH_EnableHook((LPVOID)(_targetAddr.ToPointer())); status != MH_STATUS::MH_OK && status != MH_STATUS::MH_ERROR_ENABLED)
		throw gcnew MinhookNet::Exceptions::MinhookException("Failed to enable hook.", (MHStatus)status, _targetAddr, _detourAddr, _trampolineAddr);
	_isEnabled = true;
	return true;
}

bool MinhookNet::Hooks::HookBase::Disable()
{
	if (_isDisposed)
		throw gcnew ObjectDisposedException(HookBase::GetType()->Name, "Hook has been disposed.");
	if (!_created)
		throw gcnew NullReferenceException("Hook wasn't created.");
	if (!_isEnabled)
		return false;
	if (_targetAddr == IntPtr::Zero)
		throw gcnew NullReferenceException("_targetAddr is null");
	if (auto status = MH_DisableHook(((LPVOID)_targetAddr.ToPointer())); status != MH_STATUS::MH_OK && status != MH_STATUS::MH_ERROR_DISABLED)
		throw gcnew MinhookNet::Exceptions::MinhookException("Failed to disable hook.", (MHStatus)status, _targetAddr, _detourAddr, _trampolineAddr);
	_isEnabled = false;
	return true;
}
