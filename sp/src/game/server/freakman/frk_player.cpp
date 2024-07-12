#include "cbase.h"
#include "frk_player.h"

BEGIN_DATADESC(CFreakman_Player)
DEFINE_FIELD(m_bHasWings, FIELD_BOOLEAN),
END_DATADESC()
extern int gEvilImpulse101;
LINK_ENTITY_TO_CLASS( player, CFreakman_Player );
void CFreakman_Player::CheatImpulseCommands(int iImpulse)
{
	if (!sv_cheats->GetBool())
	{
		return;
	}

	switch (iImpulse)
	{
	case 101:
		gEvilImpulse101 = true;

		EquipSuit();

		// Give the player everything!
		BaseClass::BaseClass::GiveAmmo(255, "Pistol");
		BaseClass::BaseClass::GiveAmmo(255, "AR2");
		BaseClass::BaseClass::GiveAmmo(5, "AR2AltFire");
		BaseClass::BaseClass::GiveAmmo(255, "SMG1");
		BaseClass::BaseClass::GiveAmmo(255, "Buckshot");
		BaseClass::BaseClass::GiveAmmo(3, "smg1_grenade");
		BaseClass::BaseClass::GiveAmmo(3, "rpg_round");
		BaseClass::BaseClass::GiveAmmo(5, "grenade");
		BaseClass::BaseClass::GiveAmmo(32, "357");
		BaseClass::BaseClass::GiveAmmo(16, "XBowBolt");
#ifdef HL2_EPISODIC
		BaseClass::BaseClass::GiveAmmo(5, "Hopwire");
#endif		
		/*
		GiveNamedItem( "weapon_smg1" );
		GiveNamedItem( "weapon_frag" );
		GiveNamedItem( "weapon_crowbar" );
		GiveNamedItem( "weapon_pistol" );
		GiveNamedItem( "weapon_ar2" );
		GiveNamedItem( "weapon_shotgun" );
		GiveNamedItem( "weapon_physcannon" );
		GiveNamedItem( "weapon_bugbait" );
		GiveNamedItem( "weapon_rpg" );
		GiveNamedItem( "weapon_357" );
		GiveNamedItem( "weapon_crossbow" );
		*/
		GiveNamedItem("weapon_8bit_crowbar");
		GiveNamedItem("weapon_357");
		GiveNamedItem("weapon_alyxgun");
		GiveNamedItem("weapon_annabelle");
		GiveNamedItem("weapon_antlion");
		GiveNamedItem("weapon_ar2");
		GiveNamedItem("weapon_bent_crowbar");
		GiveNamedItem("weapon_bugbait");
		GiveNamedItem("weapon_citizenpackage");
		GiveNamedItem("weapon_crossbow");
		GiveNamedItem("weapon_crowbar");
		GiveNamedItem("weapon_cubemap");
		GiveNamedItem("weapon_dual_crowbar");
		GiveNamedItem("weapon_frag");
		GiveNamedItem("weapon_glock");
		GiveNamedItem("weapon_missingpaste");
		GiveNamedItem("weapon_physcannon");
		GiveNamedItem("weapon_pistol");
		GiveNamedItem("weapon_proto1");
		GiveNamedItem("weapon_rpg");
		GiveNamedItem("weapon_shotgun");
		GiveNamedItem("weapon_shotgun_scope");
		GiveNamedItem("weapon_smg1");
		GiveNamedItem("weapon_smg2");
		GiveNamedItem("weapon_stunstick");
		GiveNamedItem("weapon_tamp");
		GiveNamedItem("weapon_physgun");
		GiveNamedItem("weapon_wand");
		GiveNamedItem("weapon_stunstick");

		EmitSound("HL2Player.TrainUse");
#ifdef HL2_EPISODIC
		// GiveNamedItem( "weapon_magnade" );
#endif
		if (GetHealth() < 100)
		{
			TakeHealth(25, DMG_GENERIC);
		}

		gEvilImpulse101 = false;

		break;
	default:
		BaseClass::CheatImpulseCommands(iImpulse);
		break;
	}
}
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
		Msg("Congratulations!\nFreakman is love, Freakman is life.\n\nMod by Wirelex.exe\nOpen Freakman project by Wirelex.exe & Gordon Freakman\n");
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
