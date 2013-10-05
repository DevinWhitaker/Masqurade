#pragma once
#include "BaseObject.h"
#include "Frame.h"

#define NPC_INTERVAL 500
enum NPC { BIRD1, BIRD2, BIRD3, CITIZEN1,CITIZEN2, NUM_NPCS };

class CNPC : public CBaseObject
{

public:

	
							
			float			m_fElapsed;//[NUM_NPCS];
							
			float			m_fNext;//[NUM_NPCS];
							
			float			m_fSpeed;//[NUM_NPCS];
							
			CFrame			m_frAnime;//[NUM_NPCS];

			CBaseObject		m_Test;//[NUM_NPCS];

	static	CNPC*		GetInstance();

			void		Update(float fElapsed);

			void		Render();

private:

	CNPC(void);
	CNPC(const CNPC&);
	CNPC& operator=(const CNPC&);
	~CNPC(void);

};

