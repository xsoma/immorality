// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "bunnyhop.h"
#include "prediction_system.h"

void bunnyhop::create_move()
{
	m_cvar()->FindVar(crypt_str("sv_autobunnyhopping"))->SetValue(g_cfg.misc.bunnyhop);
	m_cvar()->FindVar(crypt_str("sv_enablebunnyhopping"))->SetValue(g_cfg.misc.bunnyhop);

	if (g_ctx.local()->get_move_type() == MOVETYPE_LADDER)
		return;

	if ((g_ctx.get_command()->m_buttons & IN_JUMP) && !(g_ctx.local()->m_fFlags() & FL_ONGROUND)) {
		// bhop.
		if (g_cfg.misc.bunnyhop)
			g_ctx.get_command()->m_buttons &= ~IN_JUMP;

		// duck jump ( crate jump ).
		if (g_cfg.misc.crouch_in_air)
			g_ctx.get_command()->m_buttons |= IN_DUCK;
	}
}