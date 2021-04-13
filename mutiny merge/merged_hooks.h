#pragma once


#include "../includes.hpp"






using AddRenderableFn = int(__thiscall*)(void*, IClientRenderable * p_renderable, int, RenderableTranslucencyType_t, int, int);
inline AddRenderableFn oAddRenderable;

int __fastcall AddRenderable(void* ecx, void* edx, IClientRenderable* p_renderable, int unk1, RenderableTranslucencyType_t n_type, int unk2, int unk3);

using InPredictionFn = bool(__fastcall*)(void*, void*);
inline InPredictionFn oInPrediction;
bool __fastcall InPrediction(void* ecx, void* edx);


using IsPausedFn = bool(__stdcall*)();
inline IsPausedFn oIsPaused;
using PlayerFallingDamageFn = void(__thiscall*)(void* ecx);
inline PlayerFallingDamageFn oPlayerFallingDamage;

extern void autoaccept(int uMessageType);





