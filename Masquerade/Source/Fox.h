#pragma once
#include "baseobject.h"
#include "ITool.h"
#include "Powerup.h"
#include "Frame.h"
#include <vector>

class CCamera;
class CCorinne;

enum FoxAnimations { F_IDLE, F_IDLE_CORINNE, F_IDLE_SQUIRT, F_IDLE_BALLOON, F_IDLE_COTTON,
	F_WALK, F_WALK_CORINNE, F_WALK_SQUIRT, F_WALK_BALLOON, F_WALK_COTTON,
	F_JUMP, F_JUMP_CORINNE, F_JUMP_SQUIRT, F_JUMP_BALLOON, F_JUMP_COTTON,
	F_PUSH, F_PULL, F_PICKUP, F_THROW, F_FAINT, F_PAIN, F_YELL, F_NUMANIMATIONS };

class CFox : public CBaseObject
{
private:
	CCamera*				m_pCam;
	CFrame					m_frFox;
	ITool*					m_pToolInventory[ITool::MAX_TOOLS];
	CCorinne*				m_pcAttachDaughter;
	int						m_nCurTool;
	int						m_nMaxNumOfTools;
	CPowerup::PowerupType	m_ePowerupInventory[CPowerup::MAX_POWERUPS];
	CBaseObject::sDataToPassIn	m_CorinneAndFox;
	int						m_nHearts;
	float					m_fSpeedTimer;
	float					m_fThrowStrength;
//bool					m_bIsJumping;	
	float					m_fInvincibilityTimer;
	float					m_fMaxInvincibleTime;
	float					m_fNormalJumpVel;
	float					m_fDefaultJumpVel;
	float					m_fStartingX;
	float					m_fStartingY;
	float					m_fWaterDeathDelay;
	bool					m_bIsInWater;
	bool					m_bIsKnockedBack;
	vector<string>			m_vIDs;

public:
	bool					m_bInputEnabled;	CFox(void);
	virtual ~CFox(void);
	bool CheckCollision( IBaseObject * pBase );
	void Update(float fElapsed);
	void Render();
	void AddAnimation( const char* szFileName, int nAnimationToUse = 0 );
	void Input( float fElapsed );
	void HandleEvent(CEvent* pEvent);
	void SetMaxYVelocity( float mV )		{  m_fNormalJumpVel = mV;}
	void SetMaxYVelocityForReal( float mV ) { CBaseObject::SetMaxYVelocity(mV); }

	CBaseObject::sDataToPassIn	&GetData( void ) { return m_CorinneAndFox; }
	void						SetData( CBaseObject::sDataToPassIn data )
	{m_CorinneAndFox = data; }

	bool		&isInputEnabled(void)				{return m_bInputEnabled;}
	void		setInputEnabled(bool b)				{ m_bInputEnabled = b; }

	float		&GetNormalJumpVel( void )			{ return m_fNormalJumpVel;		}
	void		SetNormalJumpVel( float fJump )		{ m_fNormalJumpVel = fJump;	}
	float		GetStartingXPos( void )			{ return m_fStartingX; }
	float		GetStartingYPos( void )			{ return m_fStartingY; }	void		SetStartingXPos( float fX )		{ m_fStartingX = fX;	}	void		SetStartingYPos( float fY )		{ m_fStartingY = fY;	}	float		&GetSpeedTimer( void )			{ return m_fSpeedTimer;		}
	void		SetSpeedTimer( float fTimer )	{ m_fSpeedTimer = fTimer;	}

	int			&GetHearts( void )			{ return m_nHearts;		}
	void		SetHearts( int nHearts )	{ m_nHearts = nHearts;	}

	float		&GetStrength( void )			{ return m_fThrowStrength;		}
	void		SetStrength( float fStrength )	{ m_fThrowStrength = fStrength; }

	float		&GetMaxInvincibleTime(void)		{ return m_fMaxInvincibleTime;	}
	void		SetMaxInvincibleTime(float m)	{ m_fMaxInvincibleTime = m;		}

	float		&GetInvincibilityTimer(void)	{ return m_fInvincibilityTimer; }
	void		SetInvincibilityTimer(float i)	{ m_fInvincibilityTimer = i;	}

	void		ResetWaterDeath(void)		{SetHearts(5); m_bIsInWater = false; m_fWaterDeathDelay = 0.0f; }

	//////////////////////////////////////////////////////////////////////

	CCorinne*	&GetDaughter( void ) { return m_pcAttachDaughter; }
	void		SetDaughter( CCorinne* pCorinne ) { m_pcAttachDaughter = pCorinne; }
	//////////////////////////////////////////////////////////////////////

	CFrame		&GetFrame( void )			{ return m_frFox;		}
	int			&GetCurrentTool( void )		{ return m_nCurTool;	}
	void		SetCurrentTool(int curTool) { m_nCurTool = curTool; }
	int			GetCurrentToolType(void)	
	{ 
		if(m_nCurTool >= 0 && m_nCurTool <= ITool::MAX_TOOLS )
		{
			if(m_pToolInventory[m_nCurTool] != nullptr)
				return  m_pToolInventory[m_nCurTool]->getToolType();
			else
				return -1;
		}
		else
			return -1;
	}
	CPowerup::PowerupType	&ReturnACertainPowerUp( int nPowerUpToReturn )
	{ return m_ePowerupInventory[nPowerUpToReturn]; }
	ITool*		&GetATool( int nToolToReturn )
	{ return m_pToolInventory[ nToolToReturn ]; }

	inline void setJumpAnimation(void)
	{
		GetFrame().SetAnimationID( m_vIDs[F_JUMP] );
		GetFrame().Play();
	}

	inline bool isPlayingDead(void)
	{
		if( GetFrame().GetAnimationID() == m_vIDs[F_FAINT] )
			return GetFrame().isPlaying();
		else
			return true;
	}

	void tileCollisionResponse(int COLLISION_SIDE);

	//bool GetIsJumping() { return m_bIsJumping; }
	//void SetIsJumping( bool j ) { m_bIsJumping = j; }
	RECT GetRect();

	// Returns a pointer to the array of powerups
	// Is NOT constant, allowing modification for
	// the Powerup Menu class
	CPowerup::PowerupType* getAndSetPowerups(void) {return m_ePowerupInventory;}

	inline	void AddATool( ITool*	pTool )
	{ 
		for( int i = 0; i < ITool::MAX_TOOLS; ++i )
		{
			if( m_pToolInventory[ i ] != nullptr )
			{
				if( m_pToolInventory[ i ]->getToolType() == pTool->getToolType() )
					return;
			}
		}

		for( int i = 0; i < ITool::MAX_TOOLS; ++i )
		{
			if( m_pToolInventory[ i ] == nullptr )
			{
				m_pToolInventory[ i ] = pTool;
				++m_nMaxNumOfTools;
				break;
			}
		}
	}

	inline void clearTools(void)
	{
		for(int i = 0; i < ITool::MAX_TOOLS; ++i)
			m_pToolInventory[i] = nullptr;
		m_nMaxNumOfTools = 0;
		m_nCurTool = -1;
	}

	inline void clearPowerups(void)
	{
		for(int i = 1; i < CPowerup::MAX_POWERUPS; ++i)
			m_ePowerupInventory[i] = CPowerup::EMPTY;
	}

	inline	void AddAPowerUp( CPowerup::PowerupType	powerUp )
	{ 
		for( int i = 0; i < CPowerup::MAX_POWERUPS; ++i )
		{
			if( m_ePowerupInventory[ i ] == CPowerup::EMPTY )
			{
				m_ePowerupInventory[ i ] = powerUp; 
				break;
			}
		}
	}

	// Call this to set Fox's powerups after moving them around
	void SetPowerUps( std::vector<CPowerup::PowerupType> powerups );
	
	// CPowerup::EMPTY (-1) in any empty slots
	inline std::vector<CPowerup::PowerupType> GetPowerups(void)
	{
		std::vector<CPowerup::PowerupType> powerups;
		
		for( int i = 0; i < CPowerup::MAX_POWERUPS; ++i )
			powerups.push_back(m_ePowerupInventory[i]);
		
		return powerups;
	}

	

};

