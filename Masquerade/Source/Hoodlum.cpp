#include "Hoodlum.h"
#include "AnimationManager.h"
#include "Camera.h"
#include "Fox.h"
#include "Corinne.h"
#include "BaseAI.h"
#include "FleeState.h"
#include "SeekState.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "DestroyEnemyMessage.h"
#include "GamePlayState.h"
#include <vector>
#include <string>
using namespace std;


CHoodlum::CHoodlum(void)
{
	setEnemyType( NME_HOODLUM );

	m_nHealth = 50;

	m_bHasCorinne = false;
}

CHoodlum::CHoodlum( CHoodlum& obj)
{
	setEnemyType( NME_HOODLUM );
	m_nType = obj.m_nType;
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
	m_bHasCorinne = false;
	m_nHealth = 50;

	CSeekState* pSeek = new CSeekState();
	pSeek->AddSeekType( OBJ_CORINNE );
	pSeek->Enter( this );

	m_vIDs = CAnimationManager::GetInstance()->CreateAnimationFromFile("Resources/Scripts/MASQ_Hoodlum_Animations.xml");

	m_frAnime.Init( this );
	m_frAnime.SetAnimationID( m_vIDs[0] );
	CAnimationManager::GetInstance()->AddFrame( m_frAnime );
	m_frAnime.Play(true);
}

CHoodlum::~CHoodlum(void)
{
	CAnimationManager::GetInstance()->RemoveFrame(&m_frAnime);
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(GetCurrentState());
}

void CHoodlum::Update(float fElapsed)
{
	CCamera* cam = CCamera::GetInstance();
	RECT me = m_frAnime.GetRect();

	if( (me.right - cam->OffsetX()) < 0 || //check if im to the left of the screen
		(me.left - cam->OffsetX()) > (cam->GetScreenSpaceX() + cam->OffsetX()) || //check if im to the right of the screen
		(me.bottom - cam->OffsetY()) < 0 || // check if im above the screen
		(me.top - cam->OffsetY()) > (cam->GetScreenSpaceY() + cam->OffsetY()))
		return;//dont update if im off screen

#if _DEVIN
	//printf("Hoodlum Updating...\n");
	if( CSGD_DirectInput::GetInstance()->KeyDown( DIK_LEFTARROW ) )
		SetX( GetX() - 10 );
	if( CSGD_DirectInput::GetInstance()->KeyDown( DIK_RIGHTARROW ) )
		SetX( GetX() + 10 );
#endif

	if( GetCurrentState() != nullptr )
		GetCurrentState()->Update( fElapsed );

	float distance_between_me_and_corinne	= abs( GetX() - m_pCorinne->GetX() );
	float distance_between_me_and_fox		= abs( GetX() - m_pFox->GetX() );

	if( distance_between_me_and_corinne < FLEE_FROM_HOOD_RANGE &&
		m_pCorinne->GetCurrentState()->GetAIType() != IAIState::HELD &&
		CSGD_EventSystem::GetInstance()->HasEventTriggered("flee") == false )
	{
		m_sHoodAndCorinne.pCarrier = this;
		m_sHoodAndCorinne.pCarried = m_pCorinne;
		CSGD_EventSystem::GetInstance()->QueueEvent( "flee", &m_sHoodAndCorinne );
	}

	if( distance_between_me_and_corinne < GRAB_CORINNE_RANGE && !m_bHasCorinne && m_nHealth > 0 )
	{
		m_sHoodAndCorinne.pCarried = m_pCorinne;
		m_sHoodAndCorinne.pCarrier = this;

		if( m_pCorinne->GetCurrentState()->GetAIType() != IAIState::HELD )
		{
			if( GetX() < m_pFox->GetX() )
				m_frAnime.SetFlipped( true );
			else
				m_frAnime.SetFlipped( false );

			if( m_pCorinne->GetGreased() )//if greased pig is on
			{
				if( rand() % GRAB_CHANCE == 0 )//random chance to grab corinne
				{
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "pickup", &m_sHoodAndCorinne );
					CFleeState* pFlee = new CFleeState;
					pFlee->Enter( this );
					pFlee->AddFleeType( OBJ_FOX );
					ChangeState( pFlee );
					m_bHasCorinne = true;
					m_frAnime.SetAnimationID( m_vIDs[H_PICKUP] );
					m_frAnime.Play();
				}
			}
			else//normal conditions
			{
				CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "pickup", &m_sHoodAndCorinne );
				CFleeState* pFlee = new CFleeState;
				pFlee->Enter( this );
				pFlee->AddFleeType( OBJ_FOX );
				ChangeState( pFlee );
				m_bHasCorinne = true;
				m_frAnime.SetAnimationID( m_vIDs[H_PICKUP] );
				m_frAnime.Play();
			}
		}
		if( m_frAnime.GetAnimationID() != m_vIDs[H_RUN] )
		{
			m_frAnime.SetAnimationID( m_vIDs[H_RUN] );
			if( !m_frAnime.isPlaying() )
				m_frAnime.Play(true);
		}
	}
	else if( distance_between_me_and_fox < FLEE_FROM_FOX_RANGE && !m_bHasCorinne && m_nHealth > 0 )
	{
		if( GetCurrentState()->GetAIType() != IAIState::FLEE )
		{
			if( GetX() < m_pFox->GetX() )
				m_frAnime.SetFlipped( true );
			else
				m_frAnime.SetFlipped( false );

			CFleeState* pFlee = new CFleeState;
			pFlee->Enter( this );
			pFlee->AddFleeType( OBJ_FOX );
			ChangeState( pFlee );
			m_frAnime.SetAnimationID( m_vIDs[H_RUN] );
			if( !m_frAnime.isPlaying() )
				m_frAnime.Play(true);
		}
	}
	else if( GetCurrentState()->GetAIType() != IAIState::SEEK && !m_bHasCorinne && m_nHealth > 0 )
	{
		if( GetX() < m_pCorinne->GetX() )
			m_frAnime.SetFlipped( true );
		else
			m_frAnime.SetFlipped( false );

		CSeekState* pSeek = new CSeekState;
		pSeek->Enter( this );
		pSeek->AddSeekType( OBJ_CORINNE );
		ChangeState( pSeek );
		if( !m_frAnime.isPlaying() )
			m_frAnime.Play(true);
	}

	if( GetCurrentState()->GetAIType() == IAIState::IDLE )
	{
		m_frAnime.SetAnimationID( m_vIDs[H_IDLE] );
		if( !m_frAnime.isPlaying() )
				m_frAnime.Play(true);
	}

	if( m_nHealth <= 0 )
	{
		m_bIsAlive = false;

		if( m_frAnime.GetAnimationID() != m_vIDs[H_FAINT] )
		{
			m_frAnime.SetAnimationID( m_vIDs[H_FAINT] );
			m_frAnime.Play();
		}

		if( m_frAnime.isPlaying() == false )
		{
			CDestroyEnemyMessage* pMsg = new CDestroyEnemyMessage( this );
			CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
			CAnimationManager::GetInstance()->RemoveFrame( &m_frAnime );
			m_sHoodAndCorinne.pCarrier = nullptr;
			CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "putdown", &m_sHoodAndCorinne );
			CSGD_EventSystem::GetInstance()->UnregisterClientAll(GetCurrentState());
		}
	}

	CBaseObject::Update( fElapsed );
}

void CHoodlum::Render()
{

}

bool CHoodlum::CheckCollision(IBaseObject* pBase)
{
	return	m_frAnime.CheckCollision( (CBaseObject*)pBase );
}

void CHoodlum::HandleEvent( CEvent* pEvent )
{
	CEnemy::HandleEvent( pEvent );
}

RECT CHoodlum::GetRect()
{
	return m_frAnime.GetRect();
}