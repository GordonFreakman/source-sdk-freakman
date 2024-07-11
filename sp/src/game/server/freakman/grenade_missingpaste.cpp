//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Bugbait weapon to summon and direct antlions
//
//=============================================================================//

#include "cbase.h"
#include "grenade_missingpaste.h"
#include "decals.h"
#include "smoke_trail.h"
#include "soundent.h"
#include "engine/IEngineSound.h"
#include "npc_bullseye.h"
#include "entitylist.h"
#include "antlion_maker.h"
#include "eventqueue.h"

#ifdef PORTAL
#include "portal_util_shared.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern short	g_sModelIndexFireball;			// (in combatweapon.cpp) holds the index for the smoke cloud

// Setup the sensor list template

CEntityClassList<CMissingPasteSensor> g_MissingPasteSensorList;
template <> CMissingPasteSensor *CEntityClassList<CMissingPasteSensor>::m_pClassList = NULL;

CMissingPasteSensor* GetMissingPasteSensorList()
{
	return g_MissingPasteSensorList.m_pClassList;
}

CMissingPasteSensor::CMissingPasteSensor(void)
{
	g_MissingPasteSensorList.Insert(this);
}

CMissingPasteSensor::~CMissingPasteSensor(void)
{
	g_MissingPasteSensorList.Remove(this);
}

BEGIN_DATADESC(CMissingPasteSensor)

// This is re-set up in the constructor
//DEFINE_FIELD( m_pNext, FIELD_CLASSPTR ),

DEFINE_KEYFIELD(m_bEnabled, FIELD_BOOLEAN, "Enabled"),
DEFINE_KEYFIELD(m_flRadius, FIELD_FLOAT, "radius"),

DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),

// Function Pointers
DEFINE_OUTPUT(m_OnBaited, "OnBaited"),

END_DATADESC()

LINK_ENTITY_TO_CLASS(point_missingpaste, CMissingPasteSensor);

//=============================================================================
// Bugbait grenade
//=============================================================================

#define GRENADE_MODEL "models/weapons/w_missingpaste.mdl" //CHANGE THIS!

BEGIN_DATADESC(CGrenadeMissingPaste)

DEFINE_FIELD(m_flGracePeriodEndsAt, FIELD_TIME),
DEFINE_FIELD(m_pSporeTrail, FIELD_CLASSPTR),

// Function Pointers
DEFINE_ENTITYFUNC(MissingPasteTouch),
DEFINE_THINKFUNC(ThinkBecomeSolid),

END_DATADESC()


LINK_ENTITY_TO_CLASS(npc_grenade_missingpaste, CGrenadeMissingPaste);

//Radius of the bugbait's effect on other creatures
ConVar missingpaste_radius("missingpaste_radius", "512");
ConVar missingpaste_hear_radius("missingpaste_hear_radius", "2500");
ConVar missingpaste_distract_time("missingpaste_distract_time", "5");
ConVar missingpaste_grenade_radius("missingpaste_grenade_radius", "150");

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeMissingPaste::Spawn(void)
{
	Precache();

	SetModel(GRENADE_MODEL);
	SetCollisionGroup(COLLISION_GROUP_PROJECTILE);
	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_DEFAULT);
	SetSolid(SOLID_BBOX);

	UTIL_SetSize(this, Vector(-2, -2, -2), Vector(2, 2, 2));

	SetTouch(&CGrenadeMissingPaste::MissingPasteTouch);

	m_takedamage = DAMAGE_NO;

	m_pSporeTrail = NULL;

	/*
	m_pSporeTrail = SporeTrail::CreateSporeTrail();

	m_pSporeTrail->m_bEmit				= true;
	m_pSporeTrail->m_flSpawnRate		= 100.0f;
	m_pSporeTrail->m_flParticleLifetime	= 1.0f;
	m_pSporeTrail->m_flStartSize		= 1.0f;
	m_pSporeTrail->m_flEndSize			= 1.0f;
	m_pSporeTrail->m_flSpawnRadius		= 8.0f;

	m_pSporeTrail->m_vecEndColor		= Vector( 0, 0, 0 );
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeMissingPaste::Precache(void)
{
	PrecacheModel(GRENADE_MODEL);

	PrecacheScriptSound("GrenadeMissingPaste.Splat");

	BaseClass::Precache();
}

#define	NUM_SPLASHES	6

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeMissingPaste::MissingPasteTouch(CBaseEntity *pOther)
{
	// Don't hit triggers or water
	Assert(pOther);
	if (pOther->IsSolidFlagSet(FSOLID_TRIGGER | FSOLID_VOLUME_CONTENTS))
		return;

	if (m_pSporeTrail != NULL)
	{
		m_pSporeTrail->m_bEmit = false;
	}

	//Do effect for the hit
	/*
	SporeExplosion *pSporeExplosion = SporeExplosion::CreateSporeExplosion();

	if (pSporeExplosion)
	{
		Vector	dir = -GetAbsVelocity();
		VectorNormalize(dir);

		QAngle	angles;
		VectorAngles(dir, angles);

		pSporeExplosion->SetLocalAngles(angles);
		pSporeExplosion->SetLocalOrigin(GetAbsOrigin());
		pSporeExplosion->m_flSpawnRate = 8.0f;
		pSporeExplosion->m_flParticleLifetime = 2.0f;
		pSporeExplosion->SetRenderColor(0.0f, 0.5f, 0.25f, 0.15f);

		pSporeExplosion->m_flStartSize = 32.0f;
		pSporeExplosion->m_flEndSize = 64.0f;
		pSporeExplosion->m_flSpawnRadius = 32.0f;

		pSporeExplosion->SetLifetime(missingpaste_distract_time.GetFloat());
	}
	*/

	// Use the thrower's position as the reported position

	CPASFilter filter2(GetAbsOrigin());

	/*
	te->Explosion(filter2, -1.0,
		&GetAbsOrigin(),
		g_sModelIndexFireball,
		2.0,
		15,
		TE_EXPLFLAG_NONE,
		256.0f,
		1024.0f);
	*/

	Vector vecReported = this ? this->GetAbsOrigin() : vec3_origin;

	CTakeDamageInfo info(this, this, GetBlastForce(), GetAbsOrigin(), 128.0f, 8, 0, &vecReported);

	RadiusDamage(info, GetAbsOrigin(), 256.0f, CLASS_NONE, NULL);

	trace_t	tr;
	Vector traceDir = GetAbsVelocity();

	VectorNormalize(traceDir);

	UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + traceDir * 64, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr);

	if (tr.fraction < 1.0f)
	{
		UTIL_DecalTrace(&tr, "MissingSplash");	//dere, better decal
	}

	//Make a splat sound
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "GrenadeMissingPaste.Splat");

	//Make sure we want to call antlions
	if (ActivateMissingPasteTargets(GetThrower(), GetAbsOrigin(), false) == false)
	{
		//Alert any antlions around
		CSoundEnt::InsertSound(SOUND_BUGBAIT, GetAbsOrigin(), missingpaste_hear_radius.GetInt(), missingpaste_distract_time.GetFloat(), GetThrower());
	}

	// Tell all spawners to now fight to this position
	g_AntlionMakerManager.BroadcastFightGoal(GetAbsOrigin());

	//Go away
	UTIL_Remove(this);
}

//-----------------------------------------------------------------------------
// Purpose: Activate any nearby bugbait targets
//			Returns true if the bugbait target wants to suppress the call.
//-----------------------------------------------------------------------------
bool CGrenadeMissingPaste::ActivateMissingPasteTargets(CBaseEntity *pOwner, Vector vecOrigin, bool bSqueezed)
{
	//Attempt to activate any spawners in a radius around the bugbait
	CBaseEntity*	pList[100];
	Vector			delta(missingpaste_grenade_radius.GetFloat(), missingpaste_grenade_radius.GetFloat(), missingpaste_grenade_radius.GetFloat());
	bool			suppressCall = false;

	int count = UTIL_EntitiesInBox(pList, 100, vecOrigin - delta, vecOrigin + delta, 0);

	// If the bugbait's been thrown, look for nearby targets to affect
	if (!bSqueezed)
	{
		for (int i = 0; i < count; i++)
		{
			// If close enough, make combine soldiers freak out when hit
			if (UTIL_DistApprox(pList[i]->WorldSpaceCenter(), vecOrigin) < missingpaste_grenade_radius.GetFloat())
			{
				// Must be a soldier
				if (FClassnameIs(pList[i], "npc_combine_s"))
				{
					CAI_BaseNPC *pCombine = pList[i]->MyNPCPointer();

					if (pCombine != NULL)
					{
						trace_t tr;
						UTIL_TraceLine(vecOrigin, pCombine->EyePosition(), MASK_ALL, pOwner, COLLISION_GROUP_NONE, &tr);

						if (tr.fraction == 1.0 || tr.m_pEnt == pCombine)
						{
							// Randomize the start time a little so multiple combine hit by 
							// the same bugbait don't all dance in synch.
							g_EventQueue.AddEvent(pCombine, "HitByBugbait", RandomFloat(0, 0.5), pOwner, pOwner);
						}
					}
				}
			}
		}
	}
	// Iterate over all sensors to see if they detected this impact
	for (CMissingPasteSensor *pSensor = GetMissingPasteSensorList(); pSensor != NULL; pSensor = pSensor->m_pNext)
	{
		if (pSensor == NULL)
			continue;

		if (pSensor->IsDisabled())
			continue;

		if (bSqueezed && pSensor->DetectsSqueeze() == false)
			continue;

		if (!bSqueezed && pSensor->DetectsThrown() == false)
			continue;

		//Make sure we're within range of the sensor
		if (pSensor->GetRadius() > (pSensor->GetAbsOrigin() - vecOrigin).Length())
		{
			//Tell the sensor it's been hit
			if (pSensor->Baited(pOwner))
			{
				//If we're suppressing the call to antlions, then don't make a bugbait sound
				if (pSensor->SuppressCall())
				{
					suppressCall = true;
				}
			}
		}
	}

	return suppressCall;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeMissingPaste::ThinkBecomeSolid(void)
{
	SetThink(NULL);
	RemoveSolidFlags(FSOLID_NOT_SOLID);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : duration - 
//-----------------------------------------------------------------------------
void CGrenadeMissingPaste::SetGracePeriod(float duration)
{
	SetThink(&CGrenadeMissingPaste::ThinkBecomeSolid);
	SetNextThink(gpGlobals->curtime + duration);

	// Become unsolid	
	AddSolidFlags(FSOLID_NOT_SOLID);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &position - 
//			&angles - 
//			&velocity - 
//			&angVelocity - 
//			*owner - 
// Output : CBaseGrenade
//-----------------------------------------------------------------------------
CGrenadeMissingPaste *MissingPasteGrenade_Create(const Vector &position, const QAngle &angles, const Vector &velocity, const QAngle &angVelocity, CBaseEntity *owner)
{
	CGrenadeMissingPaste *pGrenade = (CGrenadeMissingPaste *)CBaseEntity::Create("npc_grenade_missingpaste", position, angles, owner);

	if (pGrenade != NULL)
	{
		pGrenade->SetLocalAngularVelocity(angVelocity);
		pGrenade->SetAbsVelocity(velocity);
		pGrenade->SetThrower(ToBaseCombatCharacter(owner));
	}

	return pGrenade;
}

