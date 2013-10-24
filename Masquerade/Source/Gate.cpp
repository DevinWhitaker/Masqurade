#include "Gate.h"
#include "AnimationManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"


CGate::CGate(void)
{
	m_pAM = CAnimationManager::GetInstance();
	m_vGateIDs = m_pAM->CreateAnimationFromFile("Resources/Scripts/MASQ_Gate.xml");

	m_frAnime.Init( this );
	m_frAnime.SetAnimationID( m_vGateIDs[0] );
	m_pAM->AddFrame( m_frAnime );

	m_nType = OBJ_PUZZLE;
	m_nTriggeredType = TRIGOBJ_GATE;
	m_bIsActivated = false;
	m_bIsToggleable = false;
	this->SetActivatedBool( false );
	//this->set
	RegisterDefaultEvents();
	m_szEvent = "";
#if _DEVIN
	m_szEvent = "Lever";
	m_frAnime.Play(true);
#endif
	CSGD_EventSystem::GetInstance()->RegisterClient( m_szEvent, this );


}

void CGate::SetGateFrame( unsigned int nID )

{
	if( nID < 0 || nID > 1 )
		return;
	else
		m_frAnime.SetAnimationID( m_vGateIDs[ nID ] );
}

CGate::CGate(CGate& obj)
{
	RegisterDefaultEvents();
	m_pAM = CAnimationManager::GetInstance();
	//obj.GetGateIDs() = m_pAM->CreateAnimationFromFile("Resources/Scripts/MASQ_Gate.xml");

	SetX( obj.GetX() );
	SetY( obj.GetY() );
	m_nType = obj.m_nType;
	m_vGateIDs = obj.GetGateIDs();
	m_nTriggeredType = obj.m_nTriggeredType;
	SetActivatedBool( obj.GetActivatedBool() );
	m_frAnime.Init( this );
	m_frAnime.SetAnimationID( obj.GetGateFrame().GetAnimationID() );
	m_pAM->AddFrame( m_frAnime );

	m_bIsActivated = false;
	m_bIsToggleable = false;

	m_szEvent = "";
	CSGD_EventSystem::GetInstance()->RegisterClient( m_szEvent, this );
}

CGate::~CGate(void)
{
	CAnimationManager::GetInstance()->RemoveFrame( &m_frAnime );
}

void CGate::Activate(void)
{
	m_frAnime.Play( );
	m_bIsActivated = true;
}

void CGate::Deactivate(void)
{
	/*m_frAnime.Reverse( );*/
	while( m_frAnime.GetCurFrame() != 0 )
		m_frAnime.SetCurrFrame( m_frAnime.GetCurFrame() - 1 );
	m_bIsActivated = false;
}

void CGate::Update(void)
{
}

void CGate::Render(float fElapsed)
{
}

void CGate::CollisionResponse( CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor  )
{
	if( pBase->GetType() == OBJ_FOX )
		int foo = 0;

	if( hit.left == hitbox.left )
	{
		/*if( (hit.bottom - hit.top) <= (hit.right - hit.left) )*/
		{
			int distance = hitbox.right - hitbox.left;
			pBase->SetX( pBase->GetX() - distance  );
		}
	}

	if( hit.right == hitbox.right )
	{
		/*if( (hit.bottom - hit.top) <= (hit.right - hit.left) )*/
		{
			pBase->SetX( (float)hitbox.right );
		}
	}

	if( hit.top == hitbox.top )
	{
		/*if( (hit.bottom - hit.top) >= (hit.right - hit.left) )*/
		{
			int distance = hitbox.bottom - hitbox.top;
			pBase->SetY( pBase->GetY() - distance );
		}
	}

	if( hit.bottom == hitbox.bottom )
	{
		/*if( (hit.bottom - hit.top) >= (hit.right - hit.left) )*/
		{
			pBase->SetY( (float)hitbox.top );
		}
	}
}

void CGate::HandleEvent( const CEvent* pEvent )
{
	if( pEvent->GetParam() != this )
		return;
	if( pEvent->GetEventID( ) == "activate" )
	{
		Activate();
		return;
	}
	else if( pEvent->GetEventID( ) == "deactivate")
	{
		Deactivate();
		return;
	}


	//if( pEvent->GetEventID() == m_szEvent )
	//{
	//	if( !m_bIsActivated )
	//		Activate();
	//	else if( m_bIsToggleable )
	//		Deactivate();
	//}

}

void CGate::SetEvent( string e )
{
	CSGD_EventSystem* es = CSGD_EventSystem::GetInstance();

	es->UnregisterClient( m_szEvent, this );
	m_szEvent = e;
	es->RegisterClient( m_szEvent, this );
}