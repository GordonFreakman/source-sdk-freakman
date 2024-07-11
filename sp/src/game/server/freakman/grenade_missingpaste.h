//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef GRENADE_MISSINGPASTE_H
#define GRENADE_MISSINGPASTE_H
#ifdef _WIN32
#pragma once
#endif

#include "smoke_trail.h"
#include "basegrenade_shared.h"

//Radius of the bugbait's effect on other creatures
extern ConVar missingpaste_radius;
extern ConVar missingpaste_hear_radius;
extern ConVar missingpaste_distract_time;
extern ConVar missingpaste_grenade_radius;

#define	SF_MISSINGPASTE_SUPPRESS_CALL	0x00000001
#define	SF_MISSINGPASTE_NOT_THROWN		0x00000002		// Don't detect player throwing the bugbait near this point
#define	SF_MISSINGPASTE_NOT_SQUEEZE		0x00000004		// Don't detect player squeezing the bugbait

//=============================================================================
// Bugbait sensor
//=============================================================================

class CMissingPasteSensor : public CPointEntity
{
public:

	DECLARE_CLASS(CMissingPasteSensor, CPointEntity);

	DECLARE_DATADESC();

	CMissingPasteSensor(void);
	~CMissingPasteSensor(void);

	bool Baited(CBaseEntity *pOther)
	{
		if (!m_bEnabled)
			return false;

		m_OnBaited.FireOutput(pOther, this);
		return true;
	}

	void InputEnable(inputdata_t &data)
	{
		m_bEnabled = true;
	}

	void InputDisable(inputdata_t &data)
	{
		m_bEnabled = false;
	}

	void InputToggle(inputdata_t &data)
	{
		m_bEnabled = !m_bEnabled;
	}

	bool SuppressCall(void)
	{
		return (HasSpawnFlags(SF_MISSINGPASTE_SUPPRESS_CALL));
	}

	bool DetectsSqueeze(void)
	{
		return (!HasSpawnFlags(SF_MISSINGPASTE_NOT_SQUEEZE));
	}

	bool DetectsThrown(void)
	{
		return (!HasSpawnFlags(SF_MISSINGPASTE_NOT_THROWN));
	}

	float GetRadius(void) const
	{
		if (m_flRadius == 0)
			return missingpaste_radius.GetFloat();

		return m_flRadius;
	}

	bool IsDisabled(void) const
	{
		return !m_bEnabled;
	}

protected:

	float			m_flRadius;
	bool			m_bEnabled;
	COutputEvent	m_OnBaited;

public:
	CMissingPasteSensor	*m_pNext;
};

//
// Bug Bait Grenade
//

class CGrenadeMissingPaste : public CBaseGrenade
{
	DECLARE_CLASS(CGrenadeMissingPaste, CBaseGrenade);
public:
	void	Spawn(void);
	void	Precache(void);

	void	ThinkBecomeSolid(void);
	void	SetGracePeriod(float duration);

	void	MissingPasteTouch(CBaseEntity *pOther);

	// Activate nearby bugbait targets
	static  bool	ActivateMissingPasteTargets(CBaseEntity *pOwner, Vector vecOrigin, bool bSqueezed);

	DECLARE_DATADESC();

protected:
	void	CreateTarget(const Vector &position, CBaseEntity *pOther);

	float		m_flGracePeriodEndsAt;

	SporeTrail *m_pSporeTrail;
};

extern CGrenadeMissingPaste *MissingPasteGrenade_Create(const Vector &position, const QAngle &angles, const Vector &velocity, const QAngle &angVelocity, CBaseEntity *owner);

#endif // GRENADE_BUGBAIT_H
