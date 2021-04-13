#include "merged_hooks.h"



void FlashWindow(HWND pWindow)
{
	FLASHWINFO fwInfo;
	fwInfo.cbSize = sizeof(FLASHWINFO);
	fwInfo.hwnd = pWindow;
	fwInfo.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	fwInfo.uCount = 0;
	fwInfo.dwTimeout = 0;
	FlashWindowEx(&fwInfo);
}

void SetLocalPlayerReady()
{
	using SetLocalPlayerReadyFn = void(__stdcall*)(const char*);
	static auto oSetLocalPlayerReady = reinterpret_cast<SetLocalPlayerReadyFn>(util::FindSignature("client.dll", ("55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"))); // @xref: "deffered"

	if (oSetLocalPlayerReady != nullptr)
		oSetLocalPlayerReady("");
}


void autoaccept(int uMessageType)
{
	if (g_cfg.misc.autoaccept && uMessageType == 9177)
	{
		SetLocalPlayerReady();
		Beep(500, 800);
		FlashWindow(INIT::Window);
	}
}