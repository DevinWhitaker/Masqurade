/*
	File:		IBaseObject.h
	Course:		SGP 1205
	Author:		Jam'D Team
	Purpose:	Interface and Manager for BaseObjects. 
*/ 
#pragma once

#include "IBaseObject.h"
#include "..\SGD Wrappers\IListener.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#define VELYCAP 1000
class	CBaseAI;

enum OBJECT_TYPES { OBJ_BASE, OBJ_PUZZLE, OBJ_WATER, OBJ_CHECKPOINT, OBJ_ENEMY,
			OBJ_FOX, OBJ_TOOL, OBJ_EMITTER, OBJ_TILE, OBJ_CORINNE, OBJ_BULLET, 
			OBJ_POWERUP, OBJ_TRIGGER, OBJ_BALLOON, OBJ_CRATE, OBJ_SPRING, OBJ_NPC };

enum sideOfCollision { LEFT, RIGHT, TOP, BOTTOM };

class CBaseObject :	public IBaseObject, public IListener
{
	friend class CFrame;
public:

	CBaseObject( void );
	virtual ~CBaseObject( void );

	virtual void Update( float fElapsed );
	virtual void Render( void );

	virtual bool CheckCollision( IBaseObject * pBase );
	virtual void CollisionResponse( CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor );

	virtual void HandleEvent( const CEvent* pEvent );

	void ChangeState( CBaseAI* newState );

	//void 


	struct sDataToPassIn
	{
		CBaseObject*		pCarrier;
		CBaseObject*		pCarried;
		//CBaseObject*		pSingleObject;
	};


	///////////////////////////////////////////////////////////////////
	//	Function:			"Gravity"
	//
	//	Last Modified:		5/16/2012
	//
	//	Input:				float fElapsed
	//
	//	Return:				void
	//
	//	Purpose:			Applies gravity to any object that has their boolean
	//						set to true for gravity handling.
	///////////////////////////////////////////////////////////////////
	void Gravity( float fElapsed );
	///////////////////////////////////////////////////////////////////
	//	Function:			"IncreaseVelocity"
	//
	//	Last Modified:		5/16/2012
	//
	//	Input:				float fAmountToIncrease, bool bXVelocity
	//
	//	Return:				void
	//
	//	Purpose:			Increases the x or y velocity 
	//						Passing true changes xVel and false changes yVel
	///////////////////////////////////////////////////////////////////
	void IncreaseVelocity( float fAmountToIncrease, bool bXVelocity);
	///////////////////////////////////////////////////////////////////
	//	Function:			"DecreaseVelocity"
	//
	//	Last Modified:		5/16/2012
	//
	//	Input:				float fAmountToDecrease, bool bXVelocity
	//
	//	Return:				void
	//
	//	Purpose:			Decreases the x or y velocity 
	//						Passing true changes xVel and false changes yVel
	///////////////////////////////////////////////////////////////////
	void DecreaseVelocity( float fAmountToIncrease, bool bXVelocity);
	///////////////////////////////////////////////////////////////////
	//	Function:			"HaltVelocity"
	//
	//	Last Modified:		5/16/2012
	//
	//	Input:				float fElapsed
	//
	//	Return:				void
	//
	//	Purpose:			Sets the xVelocity to 0.0f and calls the Gravity
	//						function to stop the object from moving forward.
	///////////////////////////////////////////////////////////////////
	void HaltVelocity( float fElapsed );
	///////////////////////////////////////////////////////////////////
	//	Function:			"Jump"
	//
	//	Last Modified:		5/16/2012
	//
	//	Input:				float fJumpVariable
	//
	//	Return:				void
	//
	//	Purpose:			Decreases the yVelocity until it goes above
	//						m_fMaxYVelocity.
	///////////////////////////////////////////////////////////////////
	void Jump(float fJumpVariable  );

		void SlowDown( float fElapsed );
	virtual int		GetType( void )					{ return m_nType;			}
	float			GetX( void ) const				{ return m_fX;				}
	float			GetY( void ) const				{ return m_fY;				}
	float			GetVelX( void ) const			{ return m_fVelX;			}
	float			GetVelY( void )	const			{ return m_fVelY;			}
	unsigned int	GetWidth( void ) const			{ return m_unWidth;			}		 
	unsigned int	GetHeight( void ) const			{ return m_unHeight;		}
	int				GetImageID( void ) const		{ return m_nImageID;		}
	unsigned int	GetColor( void ) const			{ return m_unColor;			}
	RECT			GetSourceRect( void ) const		{ return m_rSourceRect;		}
	virtual RECT	GetRect( );
	float	&GetMaxXVelocity( void )				{	return m_fMaxXVelocity;		}
	float	&GetMaxYVelocity( void )				{	return m_fMaxYVelocity;	}
	bool	GetGravityBool( void )					{	return m_bIsGravityOn;		}
	float	GetGravityFloat( void )					{	return m_fGravity;			}
	bool	GetJumpBool( void )						{	return m_bJumpBool;			}
	bool	GetRightBool( void )					{	return m_bRightBool;		}
	bool	GetLeftBool( void )						{	return m_bLeftBool;			}
	bool	GetTopBool( void )						{	return m_bTopBool;			}
	bool	GetStopVelocityBool( void )				{	return m_bStopVelocity;		}
	int		GetLayerType( void )					{	return m_nLayerType;		}
	std::string	&GetID( void )						{	return m_szID; }
	float	&GetMaxJumpHeight( void )				{	return m_fMaxJumpHeight;	}
	bool	&GetMovingRightBool( void )				{	return m_bMovingRight;		}
	bool	&GetJumpEnabled(void)					{	return m_bJumpEnabled;		}

	virtual void SetScalar( float timeScalar )		{	m_fTimeScalar = timeScalar;	}
	void	SetX( float x )  						{  m_fX	= x;					}
	void	SetY( float y )  						{  m_fY = y;					}
	void	SetVelX( float vx ) 					{  m_fVelX = vx;				}
	void	SetVelY( float vy )	 					{  m_fVelY = vy;				}
	void	SetWidth( unsigned int w )	 			{  m_unWidth = w;				}		 
	void	SetHeight( unsigned int h )				{  m_unHeight = h;				}
	void	SetImageID( int id )					{  m_nImageID = id;				}
	void	SetColor( unsigned int c )	 			{  m_unColor = c;				}
	void	SetSourceRect( RECT r )					{  m_rSourceRect = r;			}
	void	SetGravityBool( bool g )				{  m_bIsGravityOn = g;			}
	void	SetGravityFloat( float g )				{  m_fGravity = g;/* m_fVelY += g;*/ }
	void	SetMaxXVelocity( float mV )				{  m_fMaxXVelocity = mV;		}
	virtual void	SetMaxYVelocity( float mV )		{  m_fMaxYVelocity = mV;		}
	void	SetJumpBool( bool bIsJumping )			{	m_bJumpBool = bIsJumping;	}
	void	SetRightBool( bool bIsRight )			{	m_bRightBool = bIsRight;	}
	void	SetLeftBool( bool bIsLeft )				{	m_bLeftBool = bIsLeft;		}
	void	SetTopBool( bool bIsTop )				{	m_bTopBool = bIsTop;		}


	void	SetStopVelocityBool( bool b )			{	m_bStopVelocity = b;		}
	void	SetID( std::string szID )				{	m_szID = szID;				}
	void	SetMaxJumpHeight( float fJump )			{	m_fMaxJumpHeight = fJump;	}
	void	SetMovingRightBool( bool bMoveRight )	{	m_bMovingRight = bMoveRight; }
	void	SetJumpEnabled(bool canJump)			{   m_bJumpEnabled = canJump;	 }
	/*	
	Object Layer Type
	enum :
	1	=	FRONT_LAYER
	2	=	MID_LAYER
	3	=	BACK_LAYER
	4	=	PLAYER_LAYER
	*/
	void	SetLayerType( int nLayerNum )			{	m_nLayerType = nLayerNum;	}
	

	// "smart" pointer
	virtual void AddRef( void );
	virtual void Release( void );

	CBaseAI*	GetCurrentState( void ) { return m_pCurrObjectState; }
	void		SetCurrentState( CBaseAI* pAI ) { m_pCurrObjectState = pAI; }

	virtual void tileCollisionResponse(int COLLISION_SIDE);

protected:
	// Object Type
	int m_nType;

	// Global Time Scalar
	float m_fTimeScalar;

private:
	CBaseAI* m_pCurrObjectState;

	//	Object Layer Type
	//	enum :
	/*
		1	=	FRONT_LAYER
		2	=	MID_LAYER
		3	=	BACK_LAYER
		4	=	PLAYER_LAYER
	*/
	//string			m_szID;

	int				m_nLayerType;

	// Position
	float			m_fX;
	float			m_fY;

	// Velocity
	float			m_fVelX;
	float			m_fVelY;

	// Size
	unsigned int	 m_unWidth;
	unsigned int	 m_unHeight;

	// Image
	int				m_nImageID;
	unsigned int	m_unColor;
	RECT			m_rSourceRect;

	// Gravity
	bool			m_bIsGravityOn;

	// Reference Counter
	unsigned int	m_unRefCount;
	float			m_fWeight;

	//	Physics variables
	float			m_fMaxXVelocity;
protected:
	float			m_fMaxYVelocity;
private:
	float			m_fGravity;

	bool			m_bJumpBool;
	bool			m_bRightBool;
	bool			m_bLeftBool;
	bool			m_bTopBool;

	bool			m_bStopVelocity;
	float			m_fMaxJumpHeight;
	bool			m_bMovingRight;
	bool			m_bJumpEnabled;

public:
	bool			m_bJustColided;
private:
	//CSGD_EventSystem*	m_pES;

	//	For Moose
	std::string m_szID;
};

