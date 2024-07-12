//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Special handling for hl2 usable ladders
//
//=============================================================================//
#include "cbase.h"
#include "hl_gamemovement.h"

//TODO: freakman client class
#if defined( CLIENT_DLL )

#include "c_basehlplayer.h"
#define CFreakman_Player C_BaseHLPlayer
#else

#include "frk_player.h"	

#endif

#include "in_buttons.h"
#include "utlrbtree.h"
#include "hl2_shareddefs.h"

// Camera Bob
ConVar cl_viewbob_enabled("cl_viewbob_enabled", "1", 0, "Oscillation Toggle");
ConVar cl_viewbob_timer("cl_viewbob_timer", "10", 0, "Speed of Oscillation");
ConVar cl_viewbob_scale("cl_viewbob_scale", "0.05", 0, "Magnitude of Oscillation");

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CFreakmanGameMovement : public CHL2GameMovement
{
	typedef CGameMovement BaseClass;
public:

	CFreakmanGameMovement();
	virtual bool CheckJumpButton( void );
	void OnJump(float flImpulse);
	void WalkMove();
	CFreakman_Player	*GetFrkPlayer();
};

void CFreakmanGameMovement::WalkMove()
{
	if (cl_viewbob_enabled.GetInt() == 1 && !engine->IsPaused())
	{
		float xoffset = sin(gpGlobals->curtime * cl_viewbob_timer.GetFloat()) * player->GetAbsVelocity().Length() * cl_viewbob_scale.GetFloat() / 100;
		float yoffset = sin(2 * gpGlobals->curtime * cl_viewbob_timer.GetFloat()) * player->GetAbsVelocity().Length() * cl_viewbob_scale.GetFloat() / 400;
		player->ViewPunch(QAngle(xoffset, yoffset, 0));
	}
	BaseClass::WalkMove();
}

bool CFreakmanGameMovement::CheckJumpButton(void)
{
#ifndef CLIENT_DLL //temp
	if (!(mv->m_nOldButtons & IN_JUMP) && GetFrkPlayer()->m_bHasWings)
	{
		mv->m_vecVelocity.z += 250;
		GetFrkPlayer()->EmitSound("Player.Wing");
	}
#endif
	return BaseClass::CheckJumpButton();
}
void CFreakmanGameMovement::OnJump(float flImpulse)
{
	GetFrkPlayer()->EmitSound("Player.Jump");
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
inline CFreakman_Player	*CFreakmanGameMovement::GetFrkPlayer()
{
	return static_cast< CFreakman_Player * >(player);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFreakmanGameMovement::CFreakmanGameMovement()
{
}

// Expose our interface.
static CFreakmanGameMovement g_GameMovement;
IGameMovement *g_pGameMovement = (IGameMovement *)&g_GameMovement;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CGameMovement, IGameMovement, INTERFACENAME_GAMEMOVEMENT, g_GameMovement);