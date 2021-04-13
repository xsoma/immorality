#include "..\..\includes.hpp"

class misc : public singleton <misc> 
{
public:
	void watermark();
	void NoDuck(CUserCmd* cmd);
	void AutoCrouch(CUserCmd* cmd);
	void SlideWalk(CUserCmd* cmd);
	void automatic_peek(CUserCmd* cmd);
	void ViewModel();
	void FullBright();
	void PovArrows(player_t* e, Color color);
	void NightmodeFix();
	void desync_arrows();
	void aimbot_hitboxes();
	void ragdolls();
	void rank_reveal();
	void fast_stop(CUserCmd* m_pcmd);
	void spectators_list();
	void KillVoice(IGameEvent* event);

	bool double_tap(CUserCmd* m_pcmd);
	void hide_shots(CUserCmd* m_pcmd, bool should_work);

	bool recharging_double_tap = false;

	bool double_tap_enabled = false;
	bool double_tap_key = false;
	bool doubletap = false;


	bool hide_shots_enabled = false;
	bool hide_shots_key = false;
};