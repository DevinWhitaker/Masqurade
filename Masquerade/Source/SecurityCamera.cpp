#include "SecurityCamera.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "Camera.h"

CSecurityCamera::CSecurityCamera(void)
{
	m_nRotatingID	= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/Graphics/MASQ_CameraEye.png"));
	m_nStationsryID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/Graphics/MASQ_CameraStem.png"));
	m_fOrientation = 0.0f;
	m_fSightRange = 150;
	m_fRotationSpeed = 1.0f;
	m_fDuration = 1.9f;
	m_vLookAt.fX = 0.0f;
	m_vLookAt.fY = 0.0f;
	nDirection = 1;
	SetID("");
}


CSecurityCamera::~CSecurityCamera(void)
{
}

void CSecurityCamera::Use(void)
{

}

void CSecurityCamera::Update(float fElapsed)
{
#if _DEVIN
	CSGD_DirectInput* di = CSGD_DirectInput::GetInstance();
	if( di->KeyDown( DIK_LEFTARROW ) )
		m_fOrientation += 1 * fElapsed;
	if( di->KeyDown( DIK_RIGHTARROW ) )
		m_fOrientation -= 1 * fElapsed;
#endif

	m_fOrientation += nDirection * (( m_fRotationSpeed /  m_fDuration ) * fElapsed);

	if( m_fOrientation >= (m_fRotationSpeed *  m_fDuration) )
		nDirection = -1;
	else if( m_fOrientation <= -(m_fRotationSpeed *  m_fDuration) )
		nDirection = 1;

	m_vLookAt.fX = 0.0f;
	m_vLookAt.fY = 1.0f;
	m_vLookAt = Vector2DRotate( m_vLookAt, m_fOrientation );
	m_vLookAt = m_vLookAt * m_fSightRange;

}

void CSecurityCamera::Render()
{
	CCamera* c = CCamera::GetInstance();

	if( m_nStationsryID != -1 && m_nRotatingID != -1 )
	{
		CSGD_TextureManager::GetInstance()->Draw( m_nStationsryID, (int)((GetX() - 25) - c->OffsetX()), (int)((GetY() - 50) - c->OffsetY()) );
		CSGD_TextureManager::GetInstance()->Draw( m_nRotatingID, (int)((GetX() - 32.0f) - c->OffsetX()), (int)((GetY() - 32.0f) - c->OffsetY()), 1.0f, 1.0f, NULL, 32.0f, 32.0f, m_fOrientation );
		CSGD_Direct3D::GetInstance()->DrawLine( (int)(GetX() - c->OffsetX()), (int)(GetY() - c->OffsetY()),
			(int)(GetX() - c->OffsetX() + m_vLookAt.fX), (int)(GetY() - c->OffsetY() + m_vLookAt.fY), 0xFF0000 );
	}
}

bool CSecurityCamera::CheckCollision(IBaseObject* pBase)
{
	if( pBase->GetType() != OBJ_FOX && pBase->GetType() != OBJ_CORINNE )
		return false;

	//find the slope
	int  Slope = (int)((GetX() - (GetX() + m_vLookAt.fX)) / (GetY() - (GetY() + m_vLookAt.fY)));
	RECT collide = pBase->GetRect();

	tVector2D i = { GetX(), GetY() };
	for( float x = 0.0f; ( i.fX != m_vLookAt.fX && i.fY != m_vLookAt.fY ); x += 0.001f )
	{
		POINT l = { (LONG)i.fX, (LONG)i.fY };
		if( TRUE == PtInRect(&collide,l) )
		{
			if( GetID() != "" )
				CSGD_EventSystem::GetInstance()->QueueEvent( GetID(), this );
#if _DEVIN
			//printf("sending event: ");
			//printf(GetID().c_str());
			//printf("\n");
#endif
			return true;
		}
		
		i.fX = Lerp(GetX(),GetX() + m_vLookAt.fX,x);
		i.fY = Lerp(GetY(),GetY() + m_vLookAt.fY,x);

		if( x >= 1.0f )
			return false;
	}

	return false;
}
