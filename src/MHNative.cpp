#include "MHNative.h"
#include "StringHelper.h"
#include <vcclr.h>

MinhookNet::MHStatus MinhookNet::Native::MH_Native::MHCreateHook(IntPtr pTarget, IntPtr pDetour, [Out] IntPtr% ppOriginal)
{
	auto trampPtr = ppOriginal.ToPointer();
	auto status = MH_CreateHook(pTarget.ToPointer(), pDetour.ToPointer(), &trampPtr);
	ppOriginal = IntPtr(trampPtr);
	return (MHStatus)status;
}

MinhookNet::MHStatus MinhookNet::Native::MH_Native::MHCreateHookApi(String^ pszModule, String^ pszProcName, IntPtr pDetour, [Out] IntPtr% ppOriginal)
{
	pin_ptr<const wchar_t> pszModuleWchar = PtrToStringChars(pszModule);
	auto pszProcNameChar = StringHelper::ToC8Global(pszProcName);
	void* pOrig = ppOriginal.ToPointer();
	auto status = MH_CreateHookApi((LPCWSTR)pszModuleWchar, (LPCSTR)(pszProcNameChar.str()), pDetour.ToPointer(), &pOrig);
	ppOriginal = IntPtr(pOrig);
	return (MHStatus)status;
}

MinhookNet::MHStatus MinhookNet::Native::MH_Native::MHCreateHookApiEx(String^ pszModule, String^ pszProcName, IntPtr pDetour, [Out] IntPtr% ppOriginal, [Out] IntPtr% ppTarget)
{
	pin_ptr<const wchar_t> pszModuleWchar = PtrToStringChars(pszModule);
	auto pszProcNameChar = StringHelper::ToC8Global(pszProcName);
	void* pOrig = ppOriginal.ToPointer();
	void* pTarget = ppTarget.ToPointer();
	auto status = MH_CreateHookApiEx(pszModuleWchar, pszProcNameChar.str(), pDetour.ToPointer(), &pOrig, &pTarget);
	ppOriginal = IntPtr(pOrig);
	ppTarget = IntPtr(pTarget);
	return (MHStatus)status;
}