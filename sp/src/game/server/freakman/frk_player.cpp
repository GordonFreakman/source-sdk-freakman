#include "cbase.h"
#include "frk_player.h"

BEGIN_DATADESC(CFreakman_Player)
DEFINE_FIELD(m_bHasWings, FIELD_BOOLEAN),
END_DATADESC()

LINK_ENTITY_TO_CLASS( player, CFreakman_Player );

void CFreakman_Player::Spawn()
{
	BaseClass::Spawn();
	m_iMaxHealth = 1337;
}

bool CFreakman_Player::ClientCommand(const CCommand &args)
{
	//Let's Uncomment this just for fun.
	//#if	HL2_SINGLE_PRIMARY_WEAPON_MODE

	//Drop primary weapon
	if (!Q_stricmp(args[0], "DropPrimary"))
	{
		Weapon_DropSlot(WEAPON_PRIMARY_SLOT);
		return true;
	}

	//#endif

	if (!Q_stricmp(args[0], "freakman"))
	{
		EmitSound("Player.Freakman");
		Msg("Congratulations!\nFreakman is love, Freakman is life.\n\nMod by Wirelex.exe\nCode cleanup and open sourcing by Gordon Freakman\n");
		return true;
	}

	if (!Q_stricmp(args[0], "enable_wings"))
	{
		if (m_bHasWings == false){ m_bHasWings = true; Msg("Wings are Enabled!\n"); }
		return true;
	}
	if (!Q_stricmp(args[0], "disable_wings"))
	{
		if (m_bHasWings == true){ m_bHasWings = false; Msg("Wings are Disabled!n"); }
		return true;
	}

	if (!Q_stricmp(args[0], "enable_ljump"))
	{
		if (m_bHasWings == false){ m_bHasWings = true; Msg("Long Jump is Enabled!\n"); }
		return true;
	}
	if (!Q_stricmp(args[0], "disable_ljump"))
	{
		if (m_bHasWings == true){ m_bHasWings = false; Msg("Long Jump is Disabled!n"); }
		return true;
	}

	return BaseClass::ClientCommand(args);
}
