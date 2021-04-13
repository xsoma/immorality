#include "merged.h"
#include "NetworkedVariables.h"
#include "merged_hooks.h"
#include "console.hpp"
#include "../cheats/misc/logs.h"


template <typename T>
T* CaptureInterface(HANDLE modulehandle, const char* strInterface)
{
	typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)(modulehandle), ("CreateInterface"));
	return CreateInterface(strInterface, 0);
}


class IClientLeafSystem;
DWORD AdrOfIsPausedExtrapolate = NULL;
void mutiny_final::setup()
{
	StaticOffsets.AddAllOffsets();
	StaticOffsets.UpdateAllOffsets();
}

//small implements
bool __stdcall IsPaused()
{
	if (_ReturnAddress() == (void*)AdrOfIsPausedExtrapolate)
		return true;
	return oIsPaused();
}
void __fastcall PlayerFallingDamage(void* ecx)
{
	oPlayerFallingDamage(ecx);
	eventlogs::get().addnew("damage from fall, looser");
}
//small implements
void mutiny_final::hooks()
{
	IClientLeafSystem* ClientLeafSystem = CaptureInterface<IClientLeafSystem>(ClientHandle, ("ClientLeafSystem002"));
	AdrOfIsPausedExtrapolate = StaticOffsets.GetOffsetValue(_IsPausedExtrapolateReturnAddress);
	MH_Initialize();
	{
		const auto ClientLeafSystem_target = reinterpret_cast<void*>(get_virtual(ClientLeafSystem, 7));
		const auto InPrediction_target = reinterpret_cast<void*>(get_virtual(m_prediction(), 14));
		const auto paused_target = reinterpret_cast<void*>(get_virtual(m_engine(), 90));
		{
			MH_CreateHook(ClientLeafSystem_target, &AddRenderable, reinterpret_cast<void**>(&oAddRenderable));
			MH_CreateHook(InPrediction_target, &InPrediction, reinterpret_cast<void**>(&oInPrediction));
			MH_CreateHook(paused_target, &IsPaused, reinterpret_cast<void**>(&oIsPaused));
		}
		MH_EnableHook(MH_ALL_HOOKS);
	}
}

void mutiny_final::run()
{
	//logger::add(LSUCCESS, "reached setup point");
	setup();
	//logger::add(LSUCCESS, "reached hook point");
	hooks();
	//logger::add(LSUCCESS, "end");
}
