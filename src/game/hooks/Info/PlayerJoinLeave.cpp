#include "core/commands/BoolCommand.hpp"
#include "core/frontend/Notifications.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/backend/Self.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/rdr/Natives.hpp"
#include "network/CNetGamePlayer.hpp"

#include <player/CPlayerInfo.hpp>

namespace YimMenu::Features
{
	BoolCommand _DetectSpoofedNames{"detectspoofednames", "Detect Spoofed Names", "Detects If a Player is Possibly Spoofing Their Name"};
}
namespace YimMenu::Hooks
{
	void Info::PlayerHasJoined(CNetGamePlayer* player)
	{
		BaseHook::Get<Info::PlayerHasJoined, DetourHook<decltype(&Info::PlayerHasJoined)>>()->Original()(player);
		uint64_t rid       = player->m_PlayerInfo->m_GamerInfo.m_GamerHandle2.m_RockstarId;
		netAddress ipaddr  = player->m_PlayerInfo->m_GamerInfo.m_ExternalAddress;
		std::string ip_str = std::format("{}.{}.{}.{}", ipaddr.m_field1, ipaddr.m_field2, ipaddr.m_field3, ipaddr.m_field4);

		if (player->GetName()[0] == '~' && Features::_DetectSpoofedNames.GetState())
			Notifications::Show("Spoofed Name Detected", std::string("Spoofed Name Detected from ").append(player->GetName()), NotificationType::Warning);
		
		LOG(INFO) << std::format("{} joined the session. Reserved slot #{}. RID: {} | IP: {}", player->GetName(), (int)player->m_ActiveIndex, (int)rid, ip_str);
	}

	void Info::PlayerHasLeft(CNetGamePlayer* player)
	{
		BaseHook::Get<Info::PlayerHasLeft, DetourHook<decltype(&Info::PlayerHasLeft)>>()->Original()(player);
		uint64_t rid      = player->m_PlayerInfo->m_GamerInfo.m_GamerHandle2.m_RockstarId;
		netAddress ipaddr = player->m_PlayerInfo->m_GamerInfo.m_ExternalAddress;
		std::string ip_str = std::format("{}.{}.{}.{}", ipaddr.m_field1, ipaddr.m_field2, ipaddr.m_field3, ipaddr.m_field4);

		LOG(INFO)
		    << std::format("{} left the session. Freeing slot #{}. RID: {} | IP: {}", player->GetName(), (int)player->m_ActiveIndex, (int)rid, ip_str);
	}
}
