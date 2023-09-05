# MinhookNet
An C++/Cli wrap for for [Minhook](https://github.com/TsudaKageyu/minhook).

# Build with cmake
CMake v3.26 or above required (c++/cli /clr:netcore .NET5.0 and above).
- x32: `cmake -A Win32 -S "PathToSrc" -B "PathToBuild"`
- x64: `cmake -A x64 -S "PathToSrc" -B "PathToBuild"`

# Wrap native
MH API functions can be acsessed via ``MinhookNet.Native.MHNative``.

# 'Build-in' hooks
Besides the wrap for native Minhook API, there is build-in hook classes, wich provides same functionality but *OOP*. Some differences in compare to vanila:

### Exceptions
Unlike of returning ``MH_STATUS``, methods of ``HookBase`` class throw ``MinhookException`` exeption, wich contains error code:
```cs
using MinhookNet.Hooks;
using MinhookNet.Exceptions;

Hook _hook;
...
void InitHook()
{
	try
	{
		_hook = new(GetTargetFuncAddress(), GetDetourAddress());
		_hook.Enable();
		...
	}
	catch(MinhookException exc) 
	{
		_logger.Log(exc.ErrorStatus);
		if(exc.ErrorStatus == MinhookNet.MHStatus.ErrorNotExecutable)
			//Do smth with Win32 VirtualAlloc(...)?
		if(_hook != null)
			_hook.Dispose();
	}
}
```
### IDisposable
Build-in hooks implements ``IDisposable``. Call ``Dispose()`` to remove the hook.

# Basic usage
If you are not familiar with Minhook itself, then you probably want to read this [article](https://www.codeproject.com/Articles/44326/MinHook-The-Minimalistic-x-x-API-Hooking-Libra) :)

1. Call ``MinhookNet.Minhook.Initialize()`` at the start of your program (build-in hooks checks is MH initialized before creation, so this part can be skipped if only build-in hooks will be in use);

2. Do ur hax work;

3. Call ``MinhookNet.Minhook.Uninitialize()`` at the end of your program;

### Create a hook and remove it:
```cs
using MinhookNet;
using MinhookNet.Hooks;

static void Detour()
{
	//DoSmth
}

delegate void D();

static void Main(string[] args)
{
	D detour = Detour;
	try
	{
		hook = new Hook(GetTargetFuncAddress(), Marshal.GetFunctionPointerForDelegate(detour));
		hook.Enable();
	}
	catch(MinhookException exc){DoScaryThings(exc);}
	finally
	{
		if(hook != null) 
			hook.Dispose();//Dispose also disables the hook;
		Minhook.Uninitialize();
	}
	
}
```

### Native C equivalent:
```C
#include "Minhook.h"

void Detour()
{
	//DoSmth;
}

int main()
{
	MH_Initialize();
	uintptr_t target = get_target_func_address();
	void* trampoline;
	if(MH_CreateHook((void*)target, &Detour, &trampoline) != MH_STATUS::MH_OK)
		do_scary_things();//I'm too lazy to write this for all MH functions calls...........
	MH_EnableHook(target);
	MH_DisableHook(target);
	MH_RemoveHook(target);
	MH_Uninitialize();
	return 0;
}

```
### Again but with ``MHNative`` (why?..):
```cs
using MinhookNet;
using static MinhookNet.Native.MHNative;

static void Detour()
{
	//DoSmth
}

delegate void D();

static void Main(string[] args)
{
	D detour = Detour
	Minhook.Initialize();
	IntPtr detourAddr = Marshal.GetFunctionPointerForDelegate(detour);
	IntPtr target = GetTargetFuncAddress();
	IntPtr trampoline;
	if(MHCreateHook(target, detourAddr, out trampoline) != MHStatus.Ok)
		DoScaryThings();//I'm too lazy to write this for all MH functions calls...........
	MHEnableHook(target);
	MHDisableHook(target);
	MHRemoveHook(target);
	Minhook.Uninitialize();
}
```

# Examples

### Hook

```cs
using MinhookNet.Hooks;
namespace HookLib
{
	class Mod : IDisposable
	{
		[UnmanagedCallersOnly]
		static void DetourMethod(IntPtr obj)
		{
			//DoStuff...
		}
		
		Hook _hook;
		
		public Mod()
		{
			IntPtr targetAddr = GetTargetFunctionAddress();
			delegate* unmanaged <IntPtr, void> fp = &DetourMethod;
			try
			{
				_hook = new(targetAddr, (IntPtr)fp);
				_hook.Enable();
			}
			catch(Exception exc){...}
		}
		
		public void Dispose()
		{
			_hook.Dispose();
			...
		}
	}
}
```

### FunctionHook<T>

```cs
using MinhookNet.Hooks;
namespace HookLib
{
	class Mod : IDisposable
	{
		static void DetourMethod(IntPtr obj)
		{
			if(!_runTargetFunction)
				DoStuff();
			else
				_mod.Detour(obj)
		}
		
		//[UnmanagedFunctionPointer(CallingConvention.Cdecl)] set convention
		delegate void DetPtr(IntPtr obj);
		
		FunctionHook<DetPtr> _hook;
		
		static mod _mod;
		
		public Mod()
		{
			_mod = this;
			IntPtr targetAddr = GetTargetFunctionAddress();
			DetPtr detour = DetourMethod;
			try
			{
				_hook = new(targetAddr, detour);
				_hook.Enable();
			}
			catch(Exception exc) {...}
		}
		
		public void Dispose()
		{
			_hook.Dispose();
			...
		}
	}
}
```

### Custom asm hook

```cs
public interface IOpcodesGenerator
{
    public byte[] GenerateOpcodes();
}
```

```cs
using Minhook.Hooks;

internal class AsmHook : HookBase
{
	private IntPtr _hglobalBckp;
	
	public AsmHook(IntPtr target, IOpcodesGenerator gen) : base(target)
	{
		var opcodes = gen.GenerateOpcodes();
		_detourAddr = __hglobalBckp = Marshal.AllocHGlobal(opcodes.Lenght);
		int old;
		//PInvoke https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect
		if(!NativeExtern.VirtualProtect(_detourAddr, opcodes.Lenght, NativeExtern.MemoryProtection.Page_ExecuteReadWrite, ref old))
		{
			Marshal.FreeHGlobal(_hglobalBckp);
         throw new InvalidOperationException("Can't change memory page's descriptor");
		}
		Marshal.Copy(opcodes, 0, _detourAddr, opcodes.Length);
		CreateHook();
	}
	
	//About naming: "A_0" is autogenerated with /clr
	protected override void Dispose(bool A_0)
	{
		 if (_isDisposed)
			  return;
		 base.Dispose(A_0);
		 Marshal.FreeHGlobal(_hglobalBckp);
	}
}
```
