#ifndef FRK_PLAYER_H
#define FRK_PLAYER_H
#include "hl2_player.h"
#include "player.h"

class CFreakman_Player : public CHL2_Player
{
public:
	DECLARE_CLASS( CFreakman_Player, CHL2_Player );
	DECLARE_DATADESC();
	CFreakman_Player() {};
	~CFreakman_Player(void) {};
	
	static CFreakman_Player *CreatePlayer( const char *className, edict_t *ed )
	{
		CFreakman_Player::s_PlayerEdict = ed;
		return (CFreakman_Player*)CreateEntityByName( className );
	}
	bool m_bHasWings;
	void Spawn();
	virtual bool ClientCommand(const CCommand &args);
};

#endif