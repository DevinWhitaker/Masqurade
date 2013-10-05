#include "NPC.h"
#include "AnimationManager.h"
#include "Camera.h"
#include <vector>
#include <string>
using namespace std;

CNPC* CNPC::GetInstance()
{
	static CNPC s_Instance;

	return &s_Instance;
}

CNPC::CNPC(void)
{
	m_fElapsed = 0.0f;
	m_fSpeed = 300.0f;
	m_frAnime.Init( this );
	vector<string> ids = CAnimationManager::GetInstance()->CreateAnimationFromFile("Resources/Scripts/bird_animation.xml");
	m_frAnime.SetAnimationID( ids[0] );
	CAnimationManager::GetInstance()->AddFrame( m_frAnime );
	m_frAnime.SetXScale( 0.5f );
	m_frAnime.SetYScale( 0.5f );
	m_frAnime.Play(true);
	m_frAnime.SetFlipped( true );
	m_nType = OBJ_NPC;
}


CNPC::~CNPC(void)
{
	CAnimationManager::GetInstance()->RemoveFrame( &m_frAnime );
}

void CNPC::Update( float fElapsed )
{
	m_fElapsed += fElapsed;

	SetX( GetX() - m_fSpeed * fElapsed );

	if( m_fElapsed > m_fNext )
	{
		CCamera* cam = CCamera::GetInstance();
		int width = GetRect().right - GetRect().left;

		if( (GetX() - cam->OffsetX()) <= 0 )
		{
			m_fElapsed = 0.0f;
			m_fNext = (float)(rand() % NPC_INTERVAL);
			m_fSpeed = (rand() % 5 + 1.0f) * 12.5f;
			SetX( cam->OffsetX() + (int)cam->GetScreenSpaceX() + width );
			SetY( (float)(rand() % (int)cam->GetScreenSpaceY() / 3) );
		}
	}

	m_Test.Update( fElapsed );
}

void CNPC::Render()
{
	//m_frAnime.Play(true);
}