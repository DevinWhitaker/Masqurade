#include "Fox.h"
#include "Camera.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../Source/AnimationManager.h"
#include "Corinne.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CEvent.h"
#include "IAIState.h"
#include "HeldState.h"
#include "IdleState.h"
#include "Game.h"
#include "ObjectManager.h"
#include "Lever.h"
#include "Bullet.h"
#include "CreateEmitterMessage.h"
#include "..\SGD Wrappers\CSGD_MessageSystem.h"
#include "ResetMessage.h"
#include "GamePlayState.h"
#include "DestroyBulletMessage.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "FileManager.h"


CFox::CFox(void)
{
	this->m_nType = OBJ_FOX;
	m_pCam = CCamera::GetInstance();
	m_pcAttachDaughter = nullptr;
	SetHearts( 5 );
	SetSpeedTimer( 0.0f );
	SetStrength( 25.0f );
	SetStopVelocityBool( false );
	for(int i = 0; i < ITool::MAX_TOOLS; ++i)
		m_pToolInventory[i] = nullptr;

	m_nCurTool = -1;

	for( int i = 0; i < CPowerup::MAX_POWERUPS; ++i )
		m_ePowerupInventory[i] = CPowerup::EMPTY;

	m_nMaxNumOfTools = 0;
	this->SetJumpBool(false);
	CSGD_EventSystem::GetInstance()->RegisterClient( "Fox_Hit", this );
	m_fInvincibilityTimer = 0.0f;
	m_fMaxInvincibleTime = 3.0f;
	m_fNormalJumpVel = 0.0f;

	m_fWaterDeathDelay = 0.0f;
	m_bIsInWater = false;

	m_bIsKnockedBack = false;

	SetStartingXPos( 0.0f );
	SetStartingYPos( 0.0f );
}


CFox::~CFox(void)
{
	m_pCam = nullptr;
	CSGD_EventSystem* pES = CSGD_EventSystem::GetInstance();
	pES->UnregisterClientAll( this );
}

void CFox::Update(float fElapsed)
{
	m_fInvincibilityTimer -= fElapsed;

	if( GetCurrentState() != nullptr )
		GetCurrentState()->Update( fElapsed);
	Input( fElapsed );

	// modify the token's position by its velocity
	if( GetStopVelocityBool() == true )
		SetX( GetX() - ( GetVelX() * fElapsed) );
	else
		SetX( GetX() + ( GetVelX() * fElapsed) );

	SetY( GetY() + ( GetVelY() * fElapsed) );

	if( GetDaughter()->getPowerup() == CPowerup::FIZZY_LIFTING_DRINK )
		CBaseObject::SetMaxYVelocity( m_fNormalJumpVel );
	else if( GetDaughter()->GetCurrentState()->GetAIType() != IAIState::HELD)
		CBaseObject::SetMaxYVelocity( m_fNormalJumpVel );
	else if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this )
	{
		CBaseObject::SetMaxYVelocity( m_fNormalJumpVel *  GetDaughter()->GetFoxMoveScalar() );
	}
	else
		CBaseObject::SetMaxYVelocity( m_fNormalJumpVel );

	Gravity( fElapsed );

	if( ! m_pCam->m_bPlayerControlled )
	{
		float tolerance = 32.0f;
		float panRate = 1.0f;
		float distToFox = float( fabs(  m_pCam->GetScreenSpaceX()/2.0f - (GetX() - m_pCam->m_fXOffset) ) );
		if( m_pCam->m_fXOffset < (GetX() - (m_pCam->GetScreenSpaceX() / 2.0f)) - tolerance )
		{
			//m_pCam->m_fXOffset += GetX() - m_pCam->GetScreenSpaceX() / 2.0f;
			m_pCam->m_fXOffset += (panRate+distToFox*2.0f) * fElapsed;// - m_pCam->GetScreenSpaceX() / 2.0f;
		}

		if( m_pCam->m_fXOffset > (GetX() - (m_pCam->GetScreenSpaceX() / 2.0f)) + tolerance )
		{
  			m_pCam->m_fXOffset -= (panRate+distToFox*2.0f) * fElapsed;//GetX() - m_pCam->GetScreenSpaceX() / 2.0f;
		}
	}
	m_pCam->Update(fElapsed);

	if( GetRect().top > CGame::GetInstance()->GetHeight() )
	{
		m_bIsInWater = true;
	}
	else
	{
		m_bIsInWater = false;
	}

	if( m_fWaterDeathDelay >= 1.0f )
	{
		this->SetHearts(0);
		m_bIsInWater = false;
		m_fWaterDeathDelay = 0;
	}

	if(m_bIsInWater)
		m_fWaterDeathDelay += fElapsed;

	if( GetHearts() <= 0 )
	{
		setInputEnabled(false);
	}

	if( GetX() < 0 )
		SetX( 0 );

	if( GetY() < 0 )
		SetY( 0 );

	if( GetX() > CTileManager::GetInstance()->GetWorldWidth() )
		SetX( (float)CTileManager::GetInstance()->GetWorldWidth() );
}

void CFox::Render()
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CCamera*	cam = CCamera::GetInstance();
	CFileManager*	pFM = CFileManager::GetInstance();

	RECT me = GetDaughter()->GetFrame().GetRect();

	std::ostringstream oss;
	int fDistance = 0;
	if(  (me.left ) > (cam->GetScreenSpaceX() + cam->OffsetX())  )//check if im to the right of the screen
	{
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		if( GetDaughter() != nullptr)
			fDistance = (int)(GetDaughter()->GetX() - GetX());
		oss << fDistance << "ft.";
		if( GetDaughter()->isInflicted() == true && 
			GetDaughter() != nullptr)
		{
			std::ostringstream inflict;
			std::ostringstream helpmsg;
			helpmsg << "Corinne is in danger!";
			CBitmapFont::GetInstance()->Print( helpmsg.str().c_str(), (int)cam->GetScreenSpaceX() / 3, (int)GetDaughter()->GetY() / 2 );
			inflict << (int)(GetDaughter()->PenaltyTimeRemaining() - 490.0f) << " seconds";
			CBitmapFont::GetInstance()->Print( inflict.str().c_str(), (int)cam->GetScreenSpaceX() - 100, (int)GetDaughter()->GetY()  - 84 );
		}
		CBitmapFont::GetInstance()->Print( oss.str().c_str(), (int)cam->GetScreenSpaceX() - 100, (int)GetDaughter()->GetY()  - 64 );
		pTM->Draw( pFM->GetImageID( "MASQ_Corinne_Indicator.png" ), (int)cam->GetScreenSpaceX() - 100, (int) GetDaughter()->GetY() - 32 );
	}
	else if( (me.right - cam->OffsetX()) < 0 ) //check if im to the left of the screen)
	{
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		if( GetDaughter() != nullptr)
			fDistance = (int)(GetX() - GetDaughter()->GetX());
		oss << fDistance << "ft.";

		if( GetDaughter()->isInflicted() == true &&
			 GetDaughter() != nullptr)
		{
			std::ostringstream inflict;
			std::ostringstream helpmsg;
			helpmsg << "Corinne is in danger!";
			CBitmapFont::GetInstance()->Print( helpmsg.str().c_str(), (int)cam->GetScreenSpaceX() / 3, (int)GetDaughter()->GetY() / 2 );
			inflict << (int)(GetDaughter()->PenaltyTimeRemaining() - 490.0f) << " seconds";
			CBitmapFont::GetInstance()->Print( inflict.str().c_str(), 75, (int)GetDaughter()->GetY()  - 84 );
		}

		CBitmapFont::GetInstance()->Print( oss.str().c_str(), 75, (int)GetDaughter()->GetY()  - 64 );
		pTM->Draw( pFM->GetImageID( "MASQ_Corinne_Indicator.png" ), 50, (int) GetDaughter()->GetY() - 32 );
	}
	else
		fDistance = 0;

	/*pD3D->GetSprite()->Flush();
	RECT r = GetRect();
	r.left -= (LONG)cam->OffsetX();
	r.top -= (LONG)cam->OffsetY();
	r.right -= (LONG)cam->OffsetX();
	r.bottom -= (LONG)cam->OffsetY();
	pD3D->DrawRect( r, 255, 0, 0);*/
}

void CFox::AddAnimation( const char* szFileName, int nAnimationToUse )
{
	CAnimationManager*	pAM = CAnimationManager::GetInstance();

	m_vIDs = pAM->CreateAnimationFromFile
		( szFileName );

	GetFrame().Init( this );
	GetFrame().SetAnimationID( m_vIDs[ nAnimationToUse ] );
	CAnimationManager::GetInstance()->AddFrame(m_frFox);
}

bool CFox::CheckCollision(IBaseObject * pBase)
{
	return GetFrame().CheckCollision( (CBaseObject*)pBase );
}

void CFox::SetPowerUps( std::vector<CPowerup::PowerupType> powerups )
{
	for(unsigned int i = 0; i < CPowerup::MAX_POWERUPS; ++i)
	{
		if( i < powerups.size() )
			m_ePowerupInventory[i] = powerups[i];
		else
			m_ePowerupInventory[i] = CPowerup::EMPTY;
	}
}

void CFox::Input( float fElapsed )
{
	CSGD_DirectInput*		pDI = CSGD_DirectInput::GetInstance();
	CGame*					pGame = CGame::GetInstance();




	if(m_bInputEnabled)
	{
#if !ARCADE
		{
			if( ( pDI->KeyPressed( DIK_SPACE ) || pDI->KeyPressed( DIK_W ) ) && GetJumpEnabled() && !m_bIsKnockedBack )
			{
				SetGravityBool(true);
				if( this->GetJumpBool() != true )
				{
					switch( GetCurrentToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_SQUIRT] );
						break;
					case ITool::TOOL_BALLOONGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_BALLOON] );
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_COTTON] );
						break;
					default:
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							GetFrame().SetAnimationID( m_vIDs[F_JUMP_CORINNE] );
						else
							GetFrame().SetAnimationID( m_vIDs[F_JUMP] );
						break;
					}
					GetFrame().SetFlipped( !GetMovingRightBool() );
					GetFrame().Play();
					Jump( 50.0f );
					this->SetJumpBool( true );
				}
				return;
			}

			// =============================================
			// BUG FIX 27
			// =============================================
			// disabled fox's left and right movement when 
			// knocked back
			if(  pDI->KeyDown( DIK_D ) && ! m_bIsKnockedBack )
			{
				
				this->SetLeftBool(false);
				if( this->GetRightBool() != true )
				{
					if( GetJumpBool() == false )
					{
						if( GetFrame().isFlipped() == true )
						{
							std::vector<RECT> bounds1 = GetFrame().GetCollisionBounds();
							GetFrame().SetFlipped(false);
							std::vector<RECT> bounds2 = GetFrame().GetCollisionBounds();
							float distToMove = (float)(bounds1[0].left - bounds2[0].left);
							SetX(GetX() + distToMove);
							GetFrame().SetFlipped(true);
						}

						SetSpeedTimer( GetSpeedTimer() + fElapsed );
						IncreaseVelocity( 25.0f, true );
						SetStopVelocityBool( false );
						GetFrame().SetFlipped( false );
						switch( GetCurrentToolType() )
						{
						case ITool::TOOL_SQUIRTGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_SQUIRT] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_SQUIRT] );
							break;
						case ITool::TOOL_BALLOONGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_BALLOON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_BALLOON] );
							break;
						case ITool::TOOL_COTTONCANDYLAUNCHER:
							if( GetFrame().GetAnimationID() !=  m_vIDs[F_WALK_COTTON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_COTTON] );
							break;
						default:
							if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							{
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_CORINNE] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK_CORINNE] );
							}
							else
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK] );
							break;
						}
						/*if( !GetFrame().isPlaying() )
							GetFrame().Play();*/
						SetMovingRightBool(true);
					}

					if( GetJumpBool() == true && GetStopVelocityBool() == true )
					{
						if( GetVelX() > 55.0f )
						{
							DecreaseVelocity( 6.0f, true );
							SetStopVelocityBool(false);
							GetFrame().SetPlaying( true );
						}
						else
						{
							IncreaseVelocity( 6.5f, true );
							GetFrame().SetPlaying( true );
						}
						
					}

					if( GetJumpBool() == true && GetStopVelocityBool() == false )
					{
						IncreaseVelocity( 6.5f, true );
						GetFrame().SetPlaying( true );
					}

					// If Corinne is held
					if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD && GetJumpBool() == false )
					{
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this )
						{
							// Slow fox by her scalar (mimics him slowing down by her weight
							if(  this->GetVelY() != 0.000f )
								SetVelX( GetVelX() * (GetDaughter()->GetFoxMoveScalar()) );
						}
					}

					if( GetJumpBool() == false )
					{
						/*switch( GetCurrentToolType() )
						{
						case ITool::TOOL_SQUIRTGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_SQUIRT] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_SQUIRT] );
							break;
						case ITool::TOOL_BALLOONGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_BALLOON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_BALLOON] );
							break;
						case ITool::TOOL_COTTONCANDYLAUNCHER:
							if( GetFrame().GetAnimationID() !=  m_vIDs[F_WALK_COTTON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_COTTON] );
							break;
						default:
							if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_CORINNE] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK_CORINNE] );
							else
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK] );
							break;
						}*/
						GetFrame().SetFlipped( !GetMovingRightBool() );
						if( GetFrame().isPlaying() == false )
							GetFrame().Play( true );
					}
				}
			}
			else if( pDI->KeyDown( DIK_A ) && ! m_bIsKnockedBack )
			{
				
				this->SetRightBool(false);
				if( this->GetLeftBool() != true )
				{
					if( GetJumpBool() == false )
					{
						if( GetFrame().isFlipped() == false )
						{
							std::vector<RECT> bounds1 = GetFrame().GetCollisionBounds();
							GetFrame().SetFlipped(true);
							std::vector<RECT> bounds2 = GetFrame().GetCollisionBounds();
							float distToMove = (float)(bounds2[0].left - bounds1[0].left);
							SetX(GetX() - distToMove);
							GetFrame().SetFlipped(false);
						}
						SetSpeedTimer( GetSpeedTimer() + fElapsed );
						IncreaseVelocity( 25.0f, true );
						SetStopVelocityBool( true );
						switch( GetCurrentToolType() )
						{
						case ITool::TOOL_SQUIRTGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_SQUIRT] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_SQUIRT] );
							break;
						case ITool::TOOL_BALLOONGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_BALLOON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_BALLOON] );
							break;
						case ITool::TOOL_COTTONCANDYLAUNCHER:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_COTTON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_COTTON] );
							break;
						default:
							if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							{
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_CORINNE] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK_CORINNE] );
							}
							else
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK] );
							break;
						}
						/*if( !GetFrame().isPlaying() )
							GetFrame().Play();*/
						GetFrame().SetFlipped( true );
						SetMovingRightBool(false);
					}

					if( GetJumpBool() == true && GetStopVelocityBool() == false )
					{
						if( GetVelX() > 55.0f )
						{
							DecreaseVelocity( 6.0f, true );
							SetStopVelocityBool(true);
							GetFrame().SetPlaying( true );
						}
						else
						{
							IncreaseVelocity( 6.5f, true );
							GetFrame().SetPlaying( true );
						}
					}

					if( GetJumpBool() == true && GetStopVelocityBool() == true )
					{
						IncreaseVelocity( 6.0f, true );
						GetFrame().SetPlaying( true );
					}

					// If Corinne is held
					if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD && GetJumpBool() == false )
					{
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this )
						{
							// Slow fox by her scalar (mimics him slowing down by her weight
							if(  this->GetVelY() != 0.000f )
								SetVelX( GetVelX() * (GetDaughter()->GetFoxMoveScalar()) );
						}
					}

					if( GetJumpBool() == false )
					{
						/*switch( GetCurrentToolType() )
						{
						case ITool::TOOL_SQUIRTGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_SQUIRT] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_SQUIRT] );
							break;
						case ITool::TOOL_BALLOONGUN:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_BALLOON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_BALLOON] );
							break;
						case ITool::TOOL_COTTONCANDYLAUNCHER:
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_COTTON] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_COTTON] );
							break;
						default:
							if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							{
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_CORINNE] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK_CORINNE] );
							}
							else
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK] );
							break;
						}*/
						GetFrame().SetFlipped( !GetMovingRightBool() );
						if( GetFrame().isPlaying() == false )
							GetFrame().Play( true );
					}
				}
			}
			else if( ! m_bIsKnockedBack && GetJumpBool() == false )
			{
				SetVelX( 0.0f );
				SetSpeedTimer( 0.0f );
			}


			if( pDI->KeyPressed( DIK_E ) &&
				GetDaughter()->GetCurrentState()->GetAIType() != IAIState::HELD )
			{
				IBaseObject*	pLever = CObjectManager::GetInstance()->ReturnClosestObjectType( OBJ_TRIGGER, CTrigger::TRIG_LEVER, GetX(), GetY(), 32, 32 );

				if( pLever != nullptr )
				{
					CLever*	pLeverSwitch	= dynamic_cast< CLever* >( pLever );
					pLeverSwitch->Use( );
				}
				else
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "seek", &m_CorinneAndFox );
			}



			if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
			{
				if( pDI->MouseButtonPressed( 1 ) )
				{
					if( GetFrame().GetAnimationID() != m_vIDs[F_THROW] )
						GetFrame().SetAnimationID( m_vIDs[F_THROW] );
					if( !GetFrame().isPlaying() )
						GetFrame().Play();
					CSGD_EventSystem::GetInstance()->QueueEvent( "throw", &m_CorinneAndFox );
					return;
				}
			}


			if( m_nCurTool != -1 && pDI->MouseButtonPressed( 0 ) )
			{
				for(int i = 0; i < ITool::MAX_TOOLS; ++i)
					if( m_pToolInventory[i] != nullptr )
						m_pToolInventory[i]->SetIsSelected(false);

				if( m_pToolInventory[m_nCurTool] != nullptr )
				{
					m_pToolInventory[m_nCurTool]->SetIsSelected(true);
					m_pToolInventory[m_nCurTool]->Shoot(this);
					CSGD_XAudio2*	pXA = CSGD_XAudio2::GetInstance();
					switch( m_pToolInventory[ m_nCurTool ]->getToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						{
							pXA->SFXPlaySound( CFileManager::GetInstance()->GetSoundID( "MASQ_Squirt_Shoot" ) );
						}
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						{
						}
					default:
						break;
					};
				}

			}

			long mouseMoved = pDI->MouseWheelMovement();
			if( m_nCurTool != -1 && m_nMaxNumOfTools > 0 )
			{
				if( mouseMoved > 0 )
				{
					if( m_nCurTool == m_nMaxNumOfTools -1 )
						m_nCurTool = 0;
					else
						++m_nCurTool;
				}
				if( mouseMoved < 0 )
				{
					if( m_nCurTool == 0 )
						m_nCurTool = m_nMaxNumOfTools -1;
					else
						--m_nCurTool;
				}
			}

			if( pDI->KeyPressed( DIK_LSHIFT) )
			{
				m_CorinneAndFox.pCarrier = this;
				m_CorinneAndFox.pCarried = GetDaughter();

				if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::IDLE ||
					GetDaughter()->GetCurrentState()->GetAIType() == IAIState::FOLLOW ||
					GetDaughter()->GetCurrentState()->GetAIType() == IAIState::SEEK ||
					GetDaughter()->GetCurrentState()->GetAIType() == IAIState::FLEE )
				{
					if( GetX()  >= GetDaughter()->GetX() - 32.0f &&
						GetX() <=  GetDaughter()->GetX() + 32.0f &&
						GetY() >= GetDaughter()->GetY() - 32.0f  &&
						GetY() <= GetDaughter()->GetY() + 32.0f )
					{
						CSGD_EventSystem::GetInstance()->QueueEvent( "pickup", &m_CorinneAndFox );
						if( GetFrame().GetAnimationID() != m_vIDs[F_PICKUP] )
							GetFrame().SetAnimationID( m_vIDs[F_PICKUP] );
						if( !GetFrame().isPlaying() )
							GetFrame().Play();
					}
				}

				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
				{
					CSGD_EventSystem::GetInstance()->QueueEvent( "putdown", &m_CorinneAndFox );
					if( GetFrame().GetAnimationID() != m_vIDs[F_PICKUP] )
						GetFrame().SetAnimationID( m_vIDs[F_PICKUP] );
					GetFrame().Play();
				}
				GetFrame().SetFlipped( !GetMovingRightBool() );
				if( !GetFrame().isPlaying() )
					GetFrame().Play();
			}

			if( pDI->KeyPressed( DIK_R ) )
			{
				m_CorinneAndFox.pCarrier = this;
				m_CorinneAndFox.pCarried = GetDaughter();

				if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::FOLLOW )
					CSGD_EventSystem::GetInstance()->QueueEvent( "idle",  &m_CorinneAndFox );
				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::IDLE &&
					CSGD_EventSystem::GetInstance()->HasEventTriggered( "follow" ) == false )
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "follow",  &m_CorinneAndFox );

				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::FLEE &&
					CSGD_EventSystem::GetInstance()->HasEventTriggered( "follow" ) == false )
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "follow",  &m_CorinneAndFox );

				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::SEEK &&
					CSGD_EventSystem::GetInstance()->HasEventTriggered( "follow" ) == false )
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "follow",  &m_CorinneAndFox );

				if( GetFrame().GetAnimationID() != m_vIDs[F_YELL] )
					GetFrame().SetAnimationID( m_vIDs[F_YELL] );
				if( !GetFrame().isPlaying() )
					GetFrame().Play();
				return;
			}

			if( pDI->CheckKeys() == 0 && !GetJumpBool() )
			{
				if( !m_bIsKnockedBack )
				{
				switch( GetCurrentToolType() )
				{
				case ITool::TOOL_SQUIRTGUN:
					GetFrame().SetAnimationID( m_vIDs[F_IDLE_SQUIRT] );
					break;
				case ITool::TOOL_BALLOONGUN:
					GetFrame().SetAnimationID( m_vIDs[F_IDLE_BALLOON] );
					break;
				case ITool::TOOL_COTTONCANDYLAUNCHER:
					GetFrame().SetAnimationID( m_vIDs[F_IDLE_COTTON] );
					break;
				default:
					if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
						GetFrame().SetAnimationID( m_vIDs[F_IDLE_CORINNE] );
					else
						GetFrame().SetAnimationID( m_vIDs[F_IDLE] );
					break;
				}
				GetFrame().SetFlipped( !GetMovingRightBool() );
				if( GetFrame().isPlaying() == false )
					GetFrame().Play( true );
				}

				SetSpeedTimer( 0.0f );

				if( GetVelX() > 0 )
				{
					DecreaseVelocity( 25.0f, true );
					if( GetVelX() < 0.0f )
						SetVelX( 0.0f );
				}
				else if( GetVelX() < 0.0f )
				{
					IncreaseVelocity( 25.0f, true );
					if( GetVelX() > 0.0f )
						SetVelX( 0.0f );
				}
			}
		}
#else 
		{
			bool down = false;

			if( (pDI->JoystickGetLStickDirPressed( DIR_UP )) && GetJumpEnabled() && !m_bIsKnockedBack )
			{
				SetGravityBool(true);
				if( this->GetJumpBool() != true )
				{
					switch( GetCurrentToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_SQUIRT] );
						break;
					case ITool::TOOL_BALLOONGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_BALLOON] );
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_COTTON] );
						break;
					default:
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							GetFrame().SetAnimationID( m_vIDs[F_JUMP_CORINNE] );
						else
							GetFrame().SetAnimationID( m_vIDs[F_JUMP] );
						break;
					}
					GetFrame().SetFlipped( !GetMovingRightBool() );
					GetFrame().Play();
					Jump( 50.0f );
					this->SetJumpBool( true );
				}
				return;
			}

			else if( pDI->JoystickGetLStickDirPressed( DIR_UP ) && pDI->JoystickGetLStickDirPressed( DIR_LEFT ) && GetJumpEnabled() )
			{
				SetGravityBool(true);
				if( this->GetJumpBool() != true )
				{
					switch( GetCurrentToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_SQUIRT] );
						break;
					case ITool::TOOL_BALLOONGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_BALLOON] );
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_COTTON] );
						break;
					default:
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							GetFrame().SetAnimationID( m_vIDs[F_JUMP_CORINNE] );
						else
							GetFrame().SetAnimationID( m_vIDs[F_JUMP] );
						break;
					}
					SetStopVelocityBool( false );
					GetFrame().SetFlipped( !GetMovingRightBool() );
					GetFrame().Play();
					Jump( 50.0f );
					IncreaseVelocity( 25.0f, true );
					this->SetJumpBool( true );
				}
				return;
			}

			else if( pDI->JoystickGetLStickDirPressed( DIR_UP ) && pDI->JoystickGetLStickDirPressed( DIR_RIGHT ) && GetJumpEnabled() )
			{
				SetGravityBool(true);
				if( this->GetJumpBool() != true )
				{
					switch( GetCurrentToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_SQUIRT] );
						break;
					case ITool::TOOL_BALLOONGUN:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_BALLOON] );
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						GetFrame().SetAnimationID( m_vIDs[F_JUMP_COTTON] );
						break;
					default:
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							GetFrame().SetAnimationID( m_vIDs[F_JUMP_CORINNE] );
						else
							GetFrame().SetAnimationID( m_vIDs[F_JUMP] );
						break;
					}
					SetStopVelocityBool( true );
					GetFrame().SetFlipped( !GetMovingRightBool() );
					GetFrame().Play();
					Jump( 50.0f );
					IncreaseVelocity( 25.0f, true );
					this->SetJumpBool( true );
				}
				return;
			}


			if(  pDI->JoystickGetLStickDirDown( DIR_RIGHT ) && ! m_bIsKnockedBack )
			{

				if( GetJumpBool() == false )
				{
					SetSpeedTimer( GetSpeedTimer() + fElapsed );
					IncreaseVelocity( 25.0f, true );
					SetStopVelocityBool( false );
					GetFrame().SetFlipped( false );
					if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
						GetFrame().SetAnimationID( m_vIDs[F_WALK] );
					if( !GetFrame().isPlaying() )
						GetFrame().Play();
					GetFrame().SetPlaying( true );
					SetMovingRightBool(true);
				}

				if( GetJumpBool() == true && GetStopVelocityBool() == true )
				{
					if( GetVelX() > 55.0f )
					{
						DecreaseVelocity( 6.0f, true );
						GetFrame().SetPlaying( true );
					}
					else
					{
						IncreaseVelocity( 6.5f, true );
						GetFrame().SetPlaying( true );
					}
				}

				if( GetJumpBool() == true && GetStopVelocityBool() == false )
				{
					IncreaseVelocity( 6.0f, true );
					GetFrame().SetPlaying( true );
				}

				// If Corinne is held
				if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD && GetJumpBool() == false )
				{
					if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this )
					{
						// Slow fox by her scalar (mimics him slowing down by her weight
						if(  this->GetVelY() != 0.000f )
							SetVelX( GetVelX() * (GetDaughter()->GetFoxMoveScalar()) );
					}
				}

				if( GetJumpBool() == false )
				{
					switch( GetCurrentToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_SQUIRT] )
							GetFrame().SetAnimationID( m_vIDs[F_WALK_SQUIRT] );
						break;
					case ITool::TOOL_BALLOONGUN:
						if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_BALLOON] )
							GetFrame().SetAnimationID( m_vIDs[F_WALK_BALLOON] );
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_COTTON] )
							GetFrame().SetAnimationID( m_vIDs[F_WALK_COTTON] );
						break;
					default:
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_CORINNE] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_CORINNE] );
						else
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK] );
						break;
					}
					GetFrame().SetFlipped( !GetMovingRightBool() );
					if( GetFrame().isPlaying() == false )
						GetFrame().Play( true );
				}

				down = true;
			}
			else if( pDI->JoystickGetLStickDirDown( DIR_LEFT ) && ! m_bIsKnockedBack )
			{
				if( GetJumpBool() == false )
				{
					SetSpeedTimer( GetSpeedTimer() + fElapsed );
					IncreaseVelocity( 25.0f, true );
					SetStopVelocityBool( true );
					if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
						GetFrame().SetAnimationID( m_vIDs[F_WALK] );
					if( !GetFrame().isPlaying() )
						GetFrame().Play();
					GetFrame().SetPlaying( true );
					GetFrame().SetFlipped( true );
					SetMovingRightBool(false);
				}

				if( GetJumpBool() == true && GetStopVelocityBool() == false )
				{
					if( GetVelX() > 55.0f )
					{
						DecreaseVelocity( 6.0f, true );
						GetFrame().SetPlaying( true );
					}
					else
					{
						IncreaseVelocity( 6.5f, true );
						GetFrame().SetPlaying( true );
					}
				}

				if( GetJumpBool() == true && GetStopVelocityBool() == true )
				{
					IncreaseVelocity( 6.0f, true );
					GetFrame().SetPlaying( true );
				}

				// If Corinne is held
				if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD && GetJumpBool() == false )
				{
					if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this )
					{
						// Slow fox by her scalar (mimics him slowing down by her weight
						if(  this->GetVelY() != 0.000f )
							SetVelX( GetVelX() * (GetDaughter()->GetFoxMoveScalar()) );
					}
				}
				if(GetJumpBool() == false)
				{
					switch( GetCurrentToolType() )
					{
					case ITool::TOOL_SQUIRTGUN:
						if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_SQUIRT] )
							GetFrame().SetAnimationID( m_vIDs[F_WALK_SQUIRT] );
						break;
					case ITool::TOOL_BALLOONGUN:
						if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_BALLOON] )
							GetFrame().SetAnimationID( m_vIDs[F_WALK_BALLOON] );
						break;
					case ITool::TOOL_COTTONCANDYLAUNCHER:
						if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_COTTON] )
							GetFrame().SetAnimationID( m_vIDs[F_WALK_COTTON] );
						break;
					default:
						if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
							if( GetFrame().GetAnimationID() != m_vIDs[F_WALK_CORINNE] )
								GetFrame().SetAnimationID( m_vIDs[F_WALK_CORINNE] );
							else
								if( GetFrame().GetAnimationID() != m_vIDs[F_WALK] )
									GetFrame().SetAnimationID( m_vIDs[F_WALK] );
						break;
					}
					GetFrame().SetFlipped( !GetMovingRightBool() );
					if( GetFrame().isPlaying() == false )
						GetFrame().Play( true );
				}

				down = true;
			}
			else if( ! m_bIsKnockedBack && GetJumpBool() == false )
			{
				SetVelX( 0.0f );
				SetSpeedTimer( 0.0f );
			}

			if( pDI->JoystickButtonDown( 1 ) &&
				GetDaughter()->GetCurrentState()->GetAIType() != IAIState::HELD )
			{
				IBaseObject*	pLever = CObjectManager::GetInstance()->ReturnClosestObjectType( OBJ_TRIGGER, CTrigger::TRIG_LEVER, GetX(), GetY(), 32, 32 );

				if( pLever != nullptr )
				{
					CLever*	pLeverSwitch	= dynamic_cast< CLever* >( pLever );
					pLeverSwitch->Use();
				}

				down = true;
			}



			if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
			{
				if( pDI->JoystickButtonPressed( 5 ) )
				{
					CSGD_EventSystem::GetInstance()->QueueEvent( "throw", &m_CorinneAndFox );
					return;
				}
			}


			if( m_nCurTool != -1 && pDI->JoystickButtonPressed( 0 ) )
			{
				for(int i = 0; i < ITool::MAX_TOOLS; ++i)
					if( m_pToolInventory[i] != nullptr )
						m_pToolInventory[i]->SetIsSelected(false);

				if( m_pToolInventory[m_nCurTool] != nullptr )
				{
					m_pToolInventory[m_nCurTool]->SetIsSelected(true);
					m_pToolInventory[m_nCurTool]->Shoot(this);
				}

			}

			bool mouseMoved = pDI->JoystickGetLStickDirPressed( DIR_DOWN );
			if( m_nCurTool != -1 && m_nMaxNumOfTools > 0 )
			{
				if( mouseMoved )
				{
					if( m_nCurTool == m_nMaxNumOfTools -1 )
						m_nCurTool = 0;
					else
						++m_nCurTool;
				}

			}

			if( pDI->JoystickButtonPressed( 4 ) )
			{
				m_CorinneAndFox.pCarrier = this;
				m_CorinneAndFox.pCarried = GetDaughter();

				if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::IDLE ||
					GetDaughter()->GetCurrentState()->GetAIType() == IAIState::FOLLOW ||
					GetDaughter()->GetCurrentState()->GetAIType() == IAIState::SEEK )
				{
					CSGD_EventSystem::GetInstance()->QueueEvent( "pickup", &m_CorinneAndFox );
					GetFrame().SetAnimationID( m_vIDs[F_PICKUP] );
				}

				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
				{
					CSGD_EventSystem::GetInstance()->QueueEvent( "putdown", &m_CorinneAndFox );
					GetFrame().SetAnimationID( m_vIDs[F_PICKUP] );
				}
				GetFrame().SetFlipped( !GetMovingRightBool() );
				if( !GetFrame().isPlaying() )
					GetFrame().Play();
			}

			if( pDI->JoystickButtonPressed( 3 ) )
			{
				m_CorinneAndFox.pCarrier = this;
				m_CorinneAndFox.pCarried = GetDaughter();

				if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::IDLE &&
					CSGD_EventSystem::GetInstance()->HasEventTriggered( "follow" ) == false )
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "follow",  &m_CorinneAndFox );

				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::FLEE &&
					CSGD_EventSystem::GetInstance()->HasEventTriggered( "follow" ) == false )
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "follow",  &m_CorinneAndFox );

				else if( GetDaughter()->GetCurrentState()->GetAIType() == IAIState::SEEK &&
					CSGD_EventSystem::GetInstance()->HasEventTriggered( "follow" ) == false )
					CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "follow",  &m_CorinneAndFox );
				return;
			}

			if( pDI->JoystickCheckBufferedButtons() == -1 && !GetJumpBool() && down == false )
			{
				if( !m_bIsKnockedBack )
				{
				switch( GetCurrentToolType() )
				{
				case ITool::TOOL_SQUIRTGUN:
					GetFrame().SetAnimationID( m_vIDs[F_IDLE_SQUIRT] );
					break;
				case ITool::TOOL_BALLOONGUN:
					GetFrame().SetAnimationID( m_vIDs[F_IDLE_BALLOON] );
					break;
				case ITool::TOOL_COTTONCANDYLAUNCHER:
					GetFrame().SetAnimationID( m_vIDs[F_IDLE_COTTON] );
					break;
				default:
					if( GetDaughter()->GetCurrentState()->GetObjectsInvolved().pCarrier == this && GetDaughter()->GetCurrentState()->GetAIType() == IAIState::HELD )
						GetFrame().SetAnimationID( m_vIDs[F_IDLE_CORINNE] );
					else
						GetFrame().SetAnimationID( m_vIDs[F_IDLE] );
					break;
				}
				GetFrame().SetFlipped( !GetMovingRightBool() );
				if( GetFrame().isPlaying() == false )
					GetFrame().Play( true );
				}

				SetSpeedTimer( 0.0f );

				if( GetVelX() > 0 )
				{
					DecreaseVelocity( 25.0f, true );
					if( GetVelX() < 0.0f )
						SetVelX( 0.0f );
				}
				else if( GetVelX() < 0.0f )
				{
					IncreaseVelocity( 25.0f, true );
					if( GetVelX() > 0.0f )
						SetVelX( 0.0f );
				}
			}

		}
#endif



	}
}

void CFox::HandleEvent(CEvent* pEvent)
{

	if(pEvent->GetEventID() == "Fox_Hit")
	{
		if( m_fInvincibilityTimer <= 0 ) 
		{
			CBaseObject* pBase = (CBaseObject*)(pEvent->GetParam());
			m_fInvincibilityTimer = m_fMaxInvincibleTime;
			--m_nHearts;

			// =============================================
			// BUG FIX 2
			// =============================================
			// fox gets knocked back after respawning, 
			// don't knock fox back if he has no hearts
			if( GetHearts() >= 1 )
			{
				if(pBase->GetX() > GetX())
				{
					SetVelX(-200);
					SetY(GetY() - 5);
					SetVelY(-200);
				}
				else if(pBase->GetX() < GetX())
				{
					SetVelX(200);
					SetY(GetY() - 5);
					SetVelY(-200);
				}
			}


			m_bIsKnockedBack = true;
			if(pBase->GetType() == OBJ_BULLET)
			{
				CBullet* b = dynamic_cast<CBullet*>(pBase);
				if(b->getBulletType() == PIE_BULLET && GetHearts() > 1 )
				{
					CCreateEmitterMessage*	pPE = new CCreateEmitterMessage( "Pie Splatter", "Pie Splatter", b->GetX(), b->GetY() );
					CSGD_MessageSystem::GetInstance()->QueueMessage( pPE );
					pPE = nullptr;
				}

				CDestroyBulletMessage* pMsg = new CDestroyBulletMessage( b );
				CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
				pMsg = nullptr;
			}
			SetGravityBool(true);

			if( m_vIDs.size() > 0 )
			{
				if( GetHearts() >= 1 )
				{
					if( GetFrame().GetAnimationID() != m_vIDs[F_PAIN] )
						GetFrame().SetAnimationID( m_vIDs[F_PAIN] );
					if( !GetFrame().isPlaying() )
						GetFrame().Play();
				}
				else
				{
					if( GetFrame().GetAnimationID() != m_vIDs[F_FAINT] )
						GetFrame().SetAnimationID( m_vIDs[F_FAINT] );
					if( !GetFrame().isPlaying() )
						GetFrame().Play();
				}
			}
			else
				int foo = 0;
		}
	}
}

RECT CFox::GetRect( )
{
	std::vector<RECT> colisionRects = GetFrame().GetCollisionBounds();
	RECT tr = {};


	if( colisionRects.size() > 0 )
	{
#if _DEBUG
		//printf( "Left: %i ", colisionRects[0].left );
		//printf( "Right: %i \n", colisionRects[0].right );
		//printf( "Top: %i", colisionRects[0].top );
		//printf( "Bottom: %i \n\n", colisionRects[0].bottom );
		//CSGD_Direct3D::GetInstance()->DeviceBegin();

		//CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		//CSGD_Direct3D::GetInstance()->DeviceEnd();
		//RECT rRect = colisionRects[0];
		//rRect.left -=  (LONG)CCamera::GetInstance()->OffsetX();
		//rRect.right -=  (LONG)CCamera::GetInstance()->OffsetX();
		//rRect.top -=  (LONG)CCamera::GetInstance()->OffsetY();
		//rRect.bottom -=  (LONG)CCamera::GetInstance()->OffsetY();
		//CSGD_Direct3D::GetInstance()->DrawRect( rRect, 255, 255, 0  );
		//CSGD_Direct3D::GetInstance()->Present();
#endif

		return colisionRects[0];
	}
	else
		return tr;
}

void CFox::tileCollisionResponse(int COLLISION_SIDE)
{
	switch(COLLISION_SIDE)
	{
	case LEFT:
#if _DEBUG
		printf( "Fox has left collision with tile!" );
#endif
		SetVelX(0);
		SetGravityBool(true);
		break;
	case RIGHT:
		SetVelX(0);
		SetGravityBool(true);
		break;
	case TOP:
		SetVelY(0);
		m_bIsKnockedBack = false;
		break;
	case BOTTOM:
		SetVelY(0);
		SetGravityBool(true);
		break;
	default:
		break;
	}
	
}