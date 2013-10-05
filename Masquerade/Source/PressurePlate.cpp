// Project:		Masquerade
// Filename:	PressurePlate.cpp
// Author:		Jam'D Team
// Purpose:		Pressure Plate Object implementation.

#include "PressurePlate.h"
#include "FileManager.h"
#include "Camera.h"

CPressurePlate::CPressurePlate(void)
{
	//SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Pressure_Plate.png") );
	m_nType = OBJ_TRIGGER;
	m_nTriggerType = TRIG_PLATE;
}


CPressurePlate::~CPressurePlate(void)
{
}

CPressurePlate::CPressurePlate ( CPressurePlate &cp )
{
	SetX( cp.GetX() );
	SetY( cp.GetY() );
	SetVelX( cp.GetVelX() );
	SetVelY( cp.GetVelY() );
	SetGravityBool(cp.GetGravityBool());
	SetGravityFloat(cp.GetGravityFloat());
	SetWidth(cp.GetWidth());
	SetHeight(cp.GetHeight());
	SetLayerType( cp.GetLayerType() );
	this->m_nType = cp.m_nType;
	this->m_nTriggerType = cp.m_nTriggerType;
	//SetImageID( cp.GetImageID() );
}

void CPressurePlate::Activate(void)
{

}

void CPressurePlate::Deactivate(void)
{

}

void CPressurePlate::Update(float fElapsed)
{
	
	CBaseObject::Update(fElapsed);
}

void CPressurePlate::Render()
{
	if(GetImageID() != -1)
	{
		CCamera* pCam = CCamera::GetInstance();
		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		
		pTM->Draw( GetImageID(),
			(int)(GetX( )  - pCam->OffsetX( )), 
			(int)(GetY( )  - pCam->OffsetY( )));
	}
}

bool CPressurePlate::CheckCollision(IBaseObject* pBase)
{
	if( CBaseObject::CheckCollision( pBase ) == true )
	{
		CBaseObject* pObj = dynamic_cast< CBaseObject* >( pBase );

		if( pObj->GetType() == OBJ_FOX || pObj->GetType() == OBJ_CORINNE )
			SetInRangeBool( true );
		else
			SetInRangeBool( false );
		return true;
	}

	return false;
}

RECT CPressurePlate::GetRect( )
{
	RECT r;
	r.left = (LONG)(GetX());
	r.top = (LONG)(GetY() + 54);
	r.right = (LONG)(GetX()+GetWidth());
	r.bottom = (LONG)(GetY()+GetHeight());

	return r;
}