#include "MinhookException.h"

MinhookNet::Exceptions::MinhookException::MinhookException(String^ msg, MHStatus error) : Exception(msg)
{
	_errorStatus = error;
}

MinhookNet::Exceptions::MinhookException::MinhookException(String^ msg, MHStatus error, IntPtr target, IntPtr detour, IntPtr trampoline) : MinhookException(msg, error)
{
	_target = target;
	_detour = detour;
	_trampoline = trampoline;
}
