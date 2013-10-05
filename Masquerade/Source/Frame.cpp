#include "Frame.h"
#include "AnimationManager.h"
#include "BaseObject.h"
#include "AnchorAnimation.h"


CFrame::CFrame(void)
{
	m_nCurrFrame = 0;
	m_fElapsed = 0;
	m_bIsPlaying = false;
	m_bIsLooping = false;
	m_bIsFlipped = false;
	m_bReverse = false;
	m_fSpeed = 1;
	m_fScaleX = 1;
	m_fScaleY = 1;
	m_nX = nullptr;
	m_nY = nullptr;
	m_nWidth = nullptr;
	m_nHeight = nullptr;
	m_nStartFrame = 0;
	dwColor  = 4294967295UL;
	m_nAnimationID = "";
}


CFrame::~CFrame(void)
{
	m_nCurrFrame = -1;
	m_fElapsed = -1;
	m_bIsPlaying = false;
	m_bIsLooping = false;
	m_bIsFlipped = false;
	m_bReverse = false;
	m_fSpeed = -1;
	m_fScaleX = 1;
	m_fScaleY = 1;
	m_nX = nullptr;
	m_nY = nullptr;
	m_nWidth = nullptr;
	m_nHeight = nullptr;
	m_nStartFrame = -1;
	m_nAnimationID = "";
}


void CFrame::Init(CBaseObject* attach_to)
{
	m_pOwner = attach_to;
	m_nX = &attach_to->m_fX;
	m_nY = &attach_to->m_fY;
	m_nWidth = &attach_to->m_unWidth;
	m_nHeight = &attach_to->m_unHeight;
}

void CFrame::Init(float* X, float* Y, unsigned int* Width, unsigned int* Height )
{
	m_pOwner = nullptr;
	m_nX = X;
	m_nY = Y;
	m_nWidth = Width;
	m_nHeight = Height;
}

void CFrame::Update(float fElapsed)
{
	
}


void CFrame::Render()
{
	
}


void CFrame::Play(bool bLooping, float fSpeed)
{
	m_bIsPlaying = true;
	m_bIsLooping = bLooping;
	m_fSpeed	 = fSpeed;
	m_fElapsed = 0.0f;
	m_nCurrFrame = 0;
	m_nStartFrame = 0;
	m_bReverse = false;
}


void CFrame::Stop()
{
	m_bIsPlaying = false;
	m_bReverse = false;
}


void CFrame::Reume()
{
	m_bIsPlaying = true;
}


bool CFrame::CheckCollision(CBaseObject* pBase)
{
	return false;
}


bool CFrame::CheckCollision(CFrame* pBase)
{
	return false;
}


RECT CFrame::GetRect()
{
	RECT value;

	CAnimationManager* am = CAnimationManager::GetInstance();
	CAnchorAnimation* anime = (CAnchorAnimation*)am->operator[](GetAnimationID());
	
	SetRect(&value,
		(int)*m_nX - anime->GetXAnchor(m_nCurrFrame),
		(int)*m_nY - anime->GetYAnchor(m_nCurrFrame),
		(int)(*m_nX + anime->GetWidth(m_nCurrFrame)) - anime->GetXAnchor(m_nCurrFrame),
		(int)(*m_nY + anime->GetHeight(m_nCurrFrame)) - anime->GetYAnchor(m_nCurrFrame)
		);

	return value;
}

void CFrame::SetAnimationID( string id )
{
	m_nAnimationID = id;
	m_fElapsed = 0.0f;
	m_nCurrFrame = 0;
	m_nStartFrame = 0;
}

vector<RECT> CFrame::GetCollisionBounds()
{
	vector<RECT> e;
	vector<AnimationEvent*> t;
	CAnchorAnimation* anime = (CAnchorAnimation*)CAnimationManager::GetInstance()->operator[]( GetAnimationID() );
	t = anime->m_vAnchors[m_nCurrFrame]->Events;

	for(size_t i = 0; i < t.size(); i++)
	{
		RECT value;

		// Moving Left
		if( m_bIsFlipped )
		{
			RECT r = GetRect();
			int nWidth = t[i]->collision.right - t[i]->collision.left;
			SetRect(&value,
					int( r.right - t[i]->collision.right) /*+ nWidth*/,
					int((GetY() - anime->GetYAnchor(m_nCurrFrame))  + t[i]->collision.top),
					int( r.right  - t[i]->collision.left) /*+ nWidth*/,
					int((GetY() - anime->GetYAnchor(m_nCurrFrame))  + t[i]->collision.bottom ) );
		}
		// Moving Right
		else
		{
			SetRect(&value,
					int((GetX() - anime->GetXAnchor(m_nCurrFrame))  + t[i]->collision.left),
					int((GetY() - anime->GetYAnchor(m_nCurrFrame))  + t[i]->collision.top),
					int((GetX() - anime->GetXAnchor(m_nCurrFrame))  + t[i]->collision.right),
					int((GetY() - anime->GetYAnchor(m_nCurrFrame))  + t[i]->collision.bottom ) );
		}
				
		e.push_back( value );
	}

	return e;
}