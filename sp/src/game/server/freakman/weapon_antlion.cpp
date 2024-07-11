//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"

class CWeaponAntlion : public CBaseCombatWeapon
{
public:

	DECLARE_CLASS(CWeaponAntlion, CBaseCombatWeapon);

	void	Precache(void);

	bool	HasAnyAmmo(void)	{ return true; }

	DECLARE_SERVERCLASS();
};

LINK_ENTITY_TO_CLASS(weapon_antlion, CWeaponAntlion);

IMPLEMENT_SERVERCLASS_ST(CWeaponAntlion, DT_WeaponAntlion)
END_SEND_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponAntlion::Precache(void)
{
	BaseClass::Precache();
}
