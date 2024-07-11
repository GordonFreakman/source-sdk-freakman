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