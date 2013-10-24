#include "Clown.h"
#include "Fox.h"
#include "Corinne.h"
#include "AnimationManager.h"
#include "BaseAI.h"
#include "Camera.h"
#include "IdleState.h"
#include "SeekState.h"
#include "FleeState.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "CreateBulletMessage.h"
#include "DestroyEnemyMessage.h"
#include "GamePlayState.h"

CClown::CClown(void)
{
	setEnemyType( NME_CLOWN );

	m_nHealth = 100;
}

CClown::CClown( CClown& obj)
{
	setEnemyType( NME_CLOWN );

	SetColor(	obj.GetColor()	);							
	SetGravityBool(	obj.GetGravityBool()	);				
	SetGravityFloat(	obj.GetGravityFloat()	);				
	SetHeight(	obj.GetHeight()	);				
	SetID(	obj.GetID()	);				
	SetImageID(	obj.GetImageID()	);				
	SetJumpBool(	obj.GetJumpBool()	);				
	SetLayerType(	obj.GetLayerType()	);				
	SetMaxJumpHeight(	obj.GetMaxJumpHeight()	);				
	SetMaxXVelocity(	obj.GetMaxXVelocity()	);				
	SetMaxYVelocity(	obj.GetMaxYVelocity()	);
	SetSourceRect(	obj.GetSourceRect()	);				
	SetStopVelocityBool(	obj.GetStopVelocityBool()	);				
	SetVelX(	obj.GetVelX()	);				
	SetVelY(	obj.GetVelY()	);				
	SetWidth(	obj.GetWidth()	);				
	SetX(	obj.GetX()	);				
	SetY(	obj.GetY()	);	
	SetFox( CGamePlayState::GetInstance()->GetFox() );
	SetCorinne( CGamePlayState::GetInstance()->GetCorrine() );
	SetLayerType( obj.GetLayerType() );
	m_fElapsed = 0.0f;
	m_fRandPie = (float)(rand() % MAX_PIE_INTERVAL);
	m_nHealth = 100;

	CSeekState* pSeek2 = new CSeekState;
	pSeek2->AddSeekType( OBJ_FOX );
	pSeek2->Enter( this );

	//SetCurrentState( obj.GetCurrentState() );
	//this->ch

	vector<string> ids;

	m_vIDs = CAnimationManager::GetInstance()->CreateAnimationFromFile("Resources/Scripts/MASQ_Clown_Animations.xml");

	m_frAnime.Init( this );
	m_frAnime.SetAnimationID( m_vIDs[C_IDLE] );
	CAnimationManager::GetInstance()->AddFrame( m_frAnime );
	m_frAnime.Play(true);
}

CClown::~CClown(void)
{
	CAnimationManager::GetInstance()->RemoveFrame(&m_frAnime);
}

void CClown::Update(float fElapsed)
{
	CCamera* cam = CCamera::GetInstance();
	RECT me = m_frAnime.GetRect();

	if( (me.right - cam->OffsetX()) < 0 || //check if im to the left of the screen
		(me.left - cam->OffsetX()) > (cam->GetScreenSpaceX() + cam->OffsetX()) || //check if im to the right of the screen
		(me.bottom - cam->OffsetY()) < 0 || // check if im above the screen
		(me.top - cam->OffsetY()) > (cam->GetScreenSpaceY() + cam->OffsetY()))
		return;//dont update if im off screen

	m_fElapsed += fElapsed;

	GetCurrentState()->Update( fElapsed );

#if _DEVIN
	printf("Clown Updating...\n");
	if( CSGD_DirectInput::GetInstance()->KeyDown( DIK_LEFTARROW ) )
		SetX( GetX() - 10 );
	if( CSGD_DirectInput::GetInstance()->KeyDown( DIK_RIGHTARROW ) )
		SetX( GetX() + 10 );
	if( GetCurrentState()->GetAIType() == IAIState::SEEK )
		printf("Clown Seeking...\n");
#endif


	float distance_between_me_and_corinne	= abs( GetX() - m_pCorinne->GetX() );
	float distance_between_me_and_fox = abs( GetX() - m_pFox->GetX() );

	if( distance_between_me_and_corinne < FLEE_FROM_CLOWN_RANGE )
	{
		
		m_sClownAndCorinne.pCarried = this;
		m_sClownAndCorinne.pCarrier = m_pCorinne;
		CSGD_EventSystem::GetInstance()->QueueEvent( "flee", &m_sClownAndCorinne );
	}

	if( distance_between_me_and_fox < PUNCH_RANGE && m_nHealth > 0 )
	{
		//start punch animation
		if( GetCurrentState()->GetAIType() != IAIState::IDLE )
		{
			if( GetX() < m_pFox->GetX() )
				m_frAnime.SetFlipped( true );
			else
				m_frAnime.SetFlipped( false );

			CIdleState*pIdle = new CIdleState;
			pIdle->Enter( this );
			ChangeState( pIdle );
			m_frAnime.SetAnimationID( m_vIDs[C_IDLE] );
			m_frAnime.Play();
		}
	}
	else if( distance_between_me_and_fox < PIE_RANGE && m_nHealth > 0 )
	{
		ThrowPie();
		if( GetCurrentState()->GetAIType() != IAIState::IDLE )
		{
			if( GetX() < m_pFox->GetX() )
				m_frAnime.SetFlipped( true );
			else
				m_frAnime.SetFlipped( false );
			CIdleState*pIdle = new CIdleState;
			pIdle->Enter( this );
			ChangeState( pIdle );
			m_frAnime.SetAnimationID( m_vIDs[C_IDLE] );
			m_frAnime.Play();
		}
	}
	else if( m_fElapsed > m_fRandPie && m_nHealth > TOO_LOW )
	{
		if( GetCurrentState()->GetAIType() != IAIState::IDLE )
		{
			ThrowPie();
			m_fElapsed = 0.0f;
			m_fRandPie = (float)(rand() % MAX_PIE_INTERVAL);
			m_frAnime.SetAnimationID( m_vIDs[C_IDLE] );
			m_frAnime.Play();
		}
	}
	else if( GetCurrentState()->GetAIType() != IAIState::SEEK && m_nHealth > 0 )
	{
		if( GetX() < m_pFox->GetX() )
			m_frAnime.SetFlipped( true );
		else
			m_frAnime.SetFlipped( false );
		CSeekState* pSeek = new CSeekState;
		pSeek->Enter( this );
		pSeek->AddSeekType( OBJ_FOX );
		ChangeState( pSeek );
		m_frAnime.SetAnimationID( m_vIDs[C_WALK] );
		m_frAnime.Play(true);
	}

	if( m_nHealth < TOO_LOW && m_nHealth > 0 )
	{
		if( GetX() < m_pFox->GetX() )
			m_frAnime.SetFlipped( false );
		else
			m_frAnime.SetFlipped( true );

		CFleeState* pFlee = new CFleeState;
		pFlee->Enter( this );
		pFlee->AddFleeType( OBJ_FOX );
		ChangeState( pFlee );
		m_frAnime.SetAnimationID( m_vIDs[C_WALK] );
		m_frAnime.Play(true,1.5f);
	}

	if( m_nHealth <= 0 )
	{
		m_bIsAlive = false;

		if( m_frAnime.GetAnimationID() != m_vIDs[C_FAINT] )
		{
			m_frAnime.SetAnimationID( m_vIDs[C_FAINT] );
			m_frAnime.Play();
		}
		
		if( m_frAnime.isPlaying() == false )
		{
			CDestroyEnemyMessage* pMsg = new CDestroyEnemyMessage( this );
			CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
			CAnimationManager::GetInstance()->RemoveFrame( &m_frAnime );
		}
	}

	CBaseObject::Update( fElapsed );
}

void CClown::Render()
{

}

bool CClown::CheckCollision(IBaseObject* pBase)
{
	return m_frAnime.CheckCollision( (CBaseObject*)pBase );
}

void CClown::HandleEvent( const CEvent* pEvent )
{
	CEnemy::HandleEvent( pEvent );
}

RECT CClown::GetRect()
{
	return m_frAnime.GetRect();
}

void CClown::ThrowPie()
{
	if( m_fElapsed > PIE_RATE )
	{
		if( GetX() < m_pFox->GetX() )
			m_frAnime.SetFlipped( true );
		else
			m_frAnime.SetFlipped( false );

		CCreateBulletMessage* pBulletMsg = new CCreateBulletMessage(PIE_BULLET, this );
		CSGD_MessageSystem* pMS = CSGD_MessageSystem::GetInstance();
		pMS->QueueMessage(pBulletMsg);
		m_fElapsed = 0.0f;
		m_frAnime.SetAnimationID( m_vIDs[C_THROW] );
		m_frAnime.Play();
	}
}