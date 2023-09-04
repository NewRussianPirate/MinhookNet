#include <Minhook.h>
#include "MinhookException.h"

void MinhookNet::Minhook::Initialize()
{
	if (auto status = MH_Initialize(); status != MH_OK)
		throw gcnew MinhookNet::Exceptions::MinhookException("Failed to initialize Minhook.", (MHStatus)status);
	_initialized = true;
}

void MinhookNet::Minhook::Uninitialize()
{
	if (auto status = MH_Uninitialize(); status != MH_STATUS::MH_OK)
		throw gcnew MinhookNet::Exceptions::MinhookException("Failed to uninitialize Minhook.", (MHStatus)status);
	_initialized = false;
}
