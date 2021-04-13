// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "logs.h"



void eventlogs::paint_traverse()
{
    if (logs.empty())
        return;

    auto last_y = 3;

    const auto font = fonts[LOGS];
    const auto name_font = fonts[LOGS];

    for (unsigned int i = 0; i < logs.size(); i++) {
        auto& log = logs.at(i);

        if (util::epoch_time() - log.log_time > 4700) {
            float factor = (log.log_time + 5100) - util::epoch_time();
            factor /= 1000;

            auto opacity = int(255 * factor);

            if (opacity < 2) {
                logs.erase(logs.begin() + i);
                continue;
            }
            log.x -= 20 * (factor * 1.25);
            log.y -= 0 * (factor * 1.25);
        }

        const auto text = log.message.c_str();
        auto name_size = render::get().text_width(name_font, text);

        float logsBG[4] = { 0.3f, 0.3f, 0.3f, 0.6f };
        float logs[4] = { g_cfg.esp.textcolor.r() / 255, g_cfg.esp.textcolor.g() / 255, g_cfg.esp.textcolor.b() / 255, 1.f };

        render::get().rect_filled(log.x + 0, last_y + log.y + 18, name_size + 12, 16, Color(logsBG[0], logsBG[1], logsBG[2], logsBG[3]));
        render::get().rect_filled(log.x + 0, last_y + log.y + 34, name_size + 10, 1, log.color);
        render::get().text(font, log.x + 7, last_y + log.y + 20, Color(logs[0], logs[1], logs[2], logs[3]), HFONT_CENTERED_NONE, text);

        last_y += 25;
    }
}

void eventlogs::events(IGameEvent* event)
{
    static auto get_hitgroup_name = [](int hitgroup) -> std::string
    {
        switch (hitgroup)
        {
        case HITGROUP_HEAD:
            return crypt_str("head");
        case HITGROUP_CHEST:
            return crypt_str("chest");
        case HITGROUP_STOMACH:
            return crypt_str("stomach");
        case HITGROUP_LEFTARM:
            return crypt_str("left arm");
        case HITGROUP_RIGHTARM:
            return crypt_str("right arm");
        case HITGROUP_LEFTLEG:
            return crypt_str("left leg");
        case HITGROUP_RIGHTLEG:
            return crypt_str("right leg");
        default:
            return crypt_str("generic");
        }
    };

    if (g_cfg.misc.events_to_log[EVENTLOG_HIT] && !strcmp(event->GetName(), crypt_str("player_hurt")))
    {
        auto userid = event->GetInt(crypt_str("userid")), attacker = event->GetInt(crypt_str("attacker"));

        if (!userid || !attacker)
            return;

        auto userid_id = m_engine()->GetPlayerForUserID(userid), attacker_id = m_engine()->GetPlayerForUserID(attacker); //-V807

        player_info_t userid_info, attacker_info;

        if (!m_engine()->GetPlayerInfo(userid_id, &userid_info))
            return;

        if (!m_engine()->GetPlayerInfo(attacker_id, &attacker_info))
            return;

        auto m_victim = static_cast<player_t*>(m_entitylist()->GetClientEntity(userid_id));

        std::stringstream ss;

        if (attacker_id == m_engine()->GetLocalPlayer() && userid_id != m_engine()->GetLocalPlayer())
        {
            ss << crypt_str("You did ") << event->GetInt(crypt_str("dmg_health")) << crypt_str(" damage ") << crypt_str("to ") << userid_info.szName << crypt_str(" in ") << get_hitgroup_name(event->GetInt(crypt_str("hitgroup")));
            addnew(ss.str(), Color::Blue);
        }
        else if (userid_id == m_engine()->GetLocalPlayer() && attacker_id != m_engine()->GetLocalPlayer())
        {
            ss << attacker_info.szName << crypt_str(" did you ") << event->GetInt(crypt_str("dmg_health")) << crypt_str(" damage ") << crypt_str("in ") << get_hitgroup_name(event->GetInt(crypt_str("hitgroup")));

            addnew(ss.str(), Color::Red);
        }
    }

    if (g_cfg.misc.events_to_log[EVENTLOG_ITEM_PURCHASES] && !strcmp(event->GetName(), crypt_str("item_purchase")))
    {
        auto userid = event->GetInt(crypt_str("userid"));

        if (!userid)
            return;

        auto userid_id = m_engine()->GetPlayerForUserID(userid);

        player_info_t userid_info;

        if (!m_engine()->GetPlayerInfo(userid_id, &userid_info))
            return;

        auto m_player = static_cast<player_t*>(m_entitylist()->GetClientEntity(userid_id));

        if (!g_ctx.local() || !m_player)
            return;

        if (g_ctx.local() == m_player)
            g_ctx.globals.should_buy = 0;

        if (m_player->m_iTeamNum() == g_ctx.local()->m_iTeamNum())
            return;

        std::string weapon = event->GetString(crypt_str("weapon"));

        std::stringstream ss;
        ss << userid_info.szName << crypt_str(" bought ") << weapon;

        addnew(ss.str(), Color::Green);
    }

    if (g_cfg.misc.events_to_log[EVENTLOG_BOMB] && !strcmp(event->GetName(), crypt_str("bomb_beginplant")))
    {
        auto userid = event->GetInt(crypt_str("userid"));

        if (!userid)
            return;

        auto userid_id = m_engine()->GetPlayerForUserID(userid);

        player_info_t userid_info;

        if (!m_engine()->GetPlayerInfo(userid_id, &userid_info))
            return;

        auto m_player = static_cast<player_t*>(m_entitylist()->GetClientEntity(userid_id));

        if (!m_player)
            return;

        std::stringstream ss;
        ss << userid_info.szName << crypt_str(" has began planting the bomb");

        addnew(ss.str(), Color::Green);
    }

    if (g_cfg.misc.events_to_log[EVENTLOG_BOMB] && !strcmp(event->GetName(), crypt_str("bomb_begindefuse")))
    {
        auto userid = event->GetInt(crypt_str("userid"));

        if (!userid)
            return;

        auto userid_id = m_engine()->GetPlayerForUserID(userid);

        player_info_t userid_info;

        if (!m_engine()->GetPlayerInfo(userid_id, &userid_info))
            return;

        auto m_player = static_cast<player_t*>(m_entitylist()->GetClientEntity(userid_id));

        if (!m_player)
            return;

        std::stringstream ss;
        ss << userid_info.szName << crypt_str(" has began defusing the bomb ") << (event->GetBool(crypt_str("haskit")) ? crypt_str("with defuse kit") : crypt_str("without defuse kit"));

        addnew(ss.str(), Color::Green);
    }
}

void eventlogs::addnew(std::string text, Color color, bool full_display)
{
    logs.emplace_front(loginfo_t(util::epoch_time(), text, g_cfg.misc.log_color));

    if (!full_display)
        return;

    if (g_cfg.misc.log_output[EVENTLOG_OUTPUT_CONSOLE])
    {
        last_log = true;

#if RELEASE
#if BETA
        m_cvar()->ConsoleColorPrintf(Color::Pink, crypt_str("[ Immorality ] ")); //-V807
#else
        m_cvar()->ConsoleColorPrintf(Color::Pink, crypt_str("[ Immorality ] "));
#endif
#else
        m_cvar()->ConsoleColorPrintf(Color::Pink, crypt_str("[ Immorality ] ")); //-V807
#endif

        m_cvar()->ConsoleColorPrintf(g_cfg.misc.log_color, text.c_str());
        m_cvar()->ConsolePrintf(crypt_str("\n"));
    }

    if (g_cfg.misc.log_output[EVENTLOG_OUTPUT_CHAT])
    {
        static CHudChat* chat = nullptr;

        if (!chat)
            chat = util::FindHudElement <CHudChat>(crypt_str("CHudChat"));

#if RELEASE
#if BETA
        auto log = crypt_str("[ \x0CAImmorality \x01] ") + text;
        chat->chat_print(log.c_str());
#else
        auto log = crypt_str("[ \x0CImmorality \x01] ") + text;
        chat->chat_print(log.c_str());
#endif
#else
        auto log = crypt_str("[ \x0CImmorality \x01] ") + text;
        chat->chat_print(log.c_str());
#endif
    }
}


/*void eventlogs::paint_traverse()
{
	if (logs.empty())
		return;

	while (logs.size() > 10)
		logs.pop_back();

	auto last_y = 146;
	
	for (size_t i = 0; i < logs.size(); i++)
	{
		auto& log = logs.at(i);

		if (util::epoch_time() - log.log_time > 4600) 
		{
			auto factor = log.log_time + 5000.0f - (float)util::epoch_time();
			factor *= 0.001f;

			auto opacity = (int)(255.0f * factor);

			if (opacity < 2)
			{
				logs.erase(logs.begin() + i);
				continue;
			}

			log.color.SetAlpha(opacity);
			log.y -= factor * 1.25f;
		}

		last_y -= 14;

		auto logs_size_inverted = 10 - logs.size();
		render::get().text(fonts[LOGS], log.x, last_y + log.y - logs_size_inverted * 14, log.color, HFONT_CENTERED_NONE, log.message.c_str());
	}
}

void eventlogs::events(IGameEvent* event) 
{
	static auto get_hitgroup_name = [](int hitgroup) -> std::string
	{
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			return crypt_str("head");
		case HITGROUP_CHEST:
			return crypt_str("chest");
		case HITGROUP_STOMACH:
			return crypt_str("stomach");
		case HITGROUP_LEFTARM:
			return crypt_str("left arm");
		case HITGROUP_RIGHTARM:
			return crypt_str("right arm");
		case HITGROUP_LEFTLEG:
			return crypt_str("left leg");
		case HITGROUP_RIGHTLEG:
			return crypt_str("right leg");
		default:
			return crypt_str("generic");
		}
	};

	if (g_cfg.misc.events_to_log[EVENTLOG_HIT] && !strcmp(event->GetName(), crypt_str("player_hurt")))
	{
		auto userid = event->GetInt(crypt_str("userid")), attacker = event->GetInt(crypt_str("attacker"));

		if (!userid || !attacker) 
			return;

		auto userid_id = m_engine()->GetPlayerForUserID(userid), attacker_id = m_engine()->GetPlayerForUserID(attacker);

		player_info_t userid_info, attacker_info;

		if (!m_engine()->GetPlayerInfo(userid_id, &userid_info)) 
			return;

		if (!m_engine()->GetPlayerInfo(attacker_id, &attacker_info)) 
			return;

		auto m_victim = static_cast<player_t *>(m_entitylist()->GetClientEntity(userid_id));

		std::stringstream ss;

		if (attacker_id == m_engine()->GetLocalPlayer() && userid_id != m_engine()->GetLocalPlayer()) 
		{
			ss << crypt_str("Hit ") << userid_info.szName << crypt_str(" in the ") << get_hitgroup_name(event->GetInt(crypt_str("hitgroup"))) << crypt_str(" for ") << event->GetInt(crypt_str("dmg_health"));
			ss << crypt_str(" damage (") << event->GetInt(crypt_str("health")) << crypt_str(" health remaining)");

			add(ss.str());
		}
		else if (userid_id == m_engine()->GetLocalPlayer() && attacker_id != m_engine()->GetLocalPlayer()) 
		{
			ss << crypt_str("Take ") << event->GetInt(crypt_str("dmg_health")) << crypt_str(" damage from ") << attacker_info.szName << crypt_str(" in the ") << get_hitgroup_name(event->GetInt(crypt_str("hitgroup")));
			ss << crypt_str(" (") << event->GetInt(crypt_str("health")) << crypt_str(" health remaining)");

			add(ss.str());
		}
	}

	if (g_cfg.misc.events_to_log[EVENTLOG_ITEM_PURCHASES] && !strcmp(event->GetName(), crypt_str("item_purchase")))
	{
		auto userid = event->GetInt(crypt_str("userid"));

		if (!userid) 
			return;

		auto userid_id = m_engine()->GetPlayerForUserID(userid);

		player_info_t userid_info;

		if (!m_engine()->GetPlayerInfo(userid_id, &userid_info)) 
			return;

		auto m_player = static_cast<player_t *>(m_entitylist()->GetClientEntity(userid_id));

		if (!g_ctx.local() || !m_player) 
			return;

		if (g_ctx.local() == m_player)
			g_ctx.globals.should_buy = 0;

		if (m_player->m_iTeamNum() == g_ctx.local()->m_iTeamNum()) 
			return;

		std::string weapon = event->GetString(crypt_str("weapon"));
		
		std::stringstream ss;
		ss << userid_info.szName << crypt_str(" bought ") << weapon;

		add(ss.str());
	}

	if (g_cfg.misc.events_to_log[EVENTLOG_BOMB] && !strcmp(event->GetName(), crypt_str("bomb_beginplant")))
	{
		auto userid = event->GetInt(crypt_str("userid"));

		if (!userid) 
			return;

		auto userid_id = m_engine()->GetPlayerForUserID(userid);

		player_info_t userid_info;

		if (!m_engine()->GetPlayerInfo(userid_id, &userid_info)) 
			return;

		auto m_player = static_cast<player_t *>(m_entitylist()->GetClientEntity(userid_id));

		if (!m_player) 
			return;

		std::stringstream ss;
		ss << userid_info.szName << crypt_str(" has began planting the bomb");

		add(ss.str());
	}

	if (g_cfg.misc.events_to_log[EVENTLOG_BOMB] && !strcmp(event->GetName(), crypt_str("bomb_begindefuse")))
	{
		auto userid = event->GetInt(crypt_str("userid"));

		if (!userid) 
			return;

		auto userid_id = m_engine()->GetPlayerForUserID(userid);

		player_info_t userid_info;

		if (!m_engine()->GetPlayerInfo(userid_id, &userid_info)) 
			return;

		auto m_player = static_cast<player_t *>(m_entitylist()->GetClientEntity(userid_id));

		if (!m_player) 
			return;

		std::stringstream ss;
		ss << userid_info.szName << crypt_str(" has began defusing the bomb ") << (event->GetBool(crypt_str("haskit")) ? crypt_str("with defuse kit") : crypt_str("without defuse kit"));

		add(ss.str());
	}
}

void eventlogs::add(std::string text, bool full_display)
{
	logs.emplace_front(loginfo_t(util::epoch_time(), text, g_cfg.misc.log_color));

	if (!full_display)
		return;

	if (g_cfg.misc.log_output[EVENTLOG_OUTPUT_CONSOLE])
	{
		last_log = true;

		m_cvar()->ConsoleColorPrintf(Color(89,119,239,255), crypt_str("[Immorality] "));
		m_cvar()->ConsoleColorPrintf(Color::LightBlue, text.c_str());
		m_cvar()->ConsolePrintf(crypt_str("\n"));
	}

	if (g_cfg.misc.log_output[EVENTLOG_OUTPUT_CHAT])
	{
		static CHudChat* chat = nullptr;

		if (!chat)
			chat = util::FindHudElement <CHudChat> (crypt_str("CHudChat"));

		auto log = crypt_str("[ \x0CImmorality \x01] ") + text;
		chat->chat_print(log.c_str());
	}
}*/