//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:		Ice Axe - all purpose pointy beating stick
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "player.h"
#include "gamerules.h"
#include "ammodef.h"
#include "in_buttons.h"
#include "soundent.h"
#include "BaseBludgeonWeapon.h"
#include "vstdlib/random.h"

#define	WAND_RANGE	60
#define	WAND_REFIRE	0.25f

ConVar    sk_plr_dmg_wand("sk_plr_dmg_wand", "100");
ConVar    sk_npc_dmg_wand("sk_npc_dmg_wand", "100");

//-----------------------------------------------------------------------------
// CWeaponIceAxe
//-----------------------------------------------------------------------------

class CWeaponWand : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS(CWeaponWand, CBaseHLBludgeonWeapon);

	DECLARE_SERVERCLASS();

	CWeaponWand();

	float		GetRange(void)		{ return	WAND_RANGE; }
	float		GetFireRate(void)		{ return	WAND_REFIRE; }

	void		ImpactSound(bool isWorld)	{ return; }
	void		SecondaryAttack(void);
	void		AddViewKick(void);
	float		GetDamageForActivity(Activity hitActivity);
};

IMPLEMENT_SERVERCLASS_ST(CWeaponWand, DT_WeaponWand)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS(weapon_wand, CWeaponWand);
PRECACHE_WEAPON_REGISTER(weapon_wand);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponWand::CWeaponWand(void)
{
}

//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponWand::GetDamageForActivity(Activity hitActivity)
{
	if ((GetOwner() != NULL) && (GetOwner()->IsPlayer()))
		return sk_plr_dmg_wand.GetFloat();

	return sk_npc_dmg_wand.GetFloat();
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponWand::AddViewKick(void)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());;

	if (pPlayer == NULL)
		return;

	QAngle punchAng;

	punchAng.x = random->RandomFloat(2.0f, 4.0f);
	punchAng.y = random->RandomFloat(-2.0f, -1.0f);
	punchAng.z = 0.0f;

	pPlayer->ViewPunch(punchAng);
}

void CWeaponWand::SecondaryAttack(void)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
	if (pPlayer == NULL)
		return;

	SendWeaponAnim(ACT_VM_SECONDARYATTACK);
	WeaponSound(SPECIAL1);
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.5f;

	Vector vecAiming = pPlayer->GetAutoaimVector(0);
	Vector vecSrc = pPlayer->Weapon_ShootPosition();

	QAngle angAiming;
	VectorAngles(vecAiming, angAiming);

	// We're committed to the shot now. So take the ammo.
	//pOwner->RemoveAmmo(1, m_iSecondaryAmmoType);

	trace_t tr;
	// Trace the initial shot from the weapon
	UTIL_TraceLine(vecSrc, vecSrc + vecAiming * MAX_COORD_INTEGER, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr);
	NDebugOverlay::Line(vecSrc + Vector(0, 0, -3), tr.endpos, 255, 255, 0, false, 0.1f);

	if (tr.m_pEnt == NULL)
	{
		// Bail out if we hit nothing at all.
		return;
	}

	// Record the entity that the beam struck.
	CBaseEntity *pObjectStruck = tr.m_pEnt;

	if (pObjectStruck->MyNPCPointer() && pObjectStruck->IsAlive())
	{
		CTakeDamageInfo damageInfo(this, pPlayer, tr.m_pEnt->GetHealth(), DMG_GENERIC | DMG_DISSOLVE);
		Vector force(0, 0, 100.0f);
		damageInfo.SetDamageForce(force);
		damageInfo.SetDamagePosition(tr.endpos);
		tr.m_pEnt->TakeDamage(damageInfo);
	}

}