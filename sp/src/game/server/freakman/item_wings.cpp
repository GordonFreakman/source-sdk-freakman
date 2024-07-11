//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Handling for the suit batteries.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "frk_player.h"
#include "basecombatweapon.h"
#include "gamerules.h"
#include "items.h"
#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CItemWings : public CItem
{
public:
	DECLARE_CLASS(CItemWings, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/battery.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/battery.mdl");

		PrecacheScriptSound("ItemBattery.Touch");

	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		CFreakman_Player *pHL2Player = dynamic_cast<CFreakman_Player *>(pPlayer);
		if (pHL2Player->m_bHasWings == true)
		{
			return false;
		}
		else{
			pHL2Player->m_bHasWings = true;
			return true;
		}
		//return (pHL2Player && pHL2Player->ApplyBattery());
	}
};

LINK_ENTITY_TO_CLASS(item_wings, CItemWings);
PRECACHE_REGISTER(item_wings);

