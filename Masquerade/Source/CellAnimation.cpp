#include "CellAnimation.h"
#include "Game.h"
#include "Camera.h"
#include "IBaseObject.h"
#if _DEVIN
#include "../SGD Wrappers/CSGD_Direct3D.h"
#endif

CCellAnimation::CCellAnimation(void)
{
	m_pTM = nullptr;
	m_nWidth = -1;
	m_nHeight = -1;
	m_nMaxFrames = -1;
	m_nRows = -1;
	m_nCols = -1;
	m_nImageID = -1;
}


CCellAnimation::~CCellAnimation(void)
{
	m_pTM = nullptr;
	m_nWidth = -1;
	m_nHeight = -1;
	m_nMaxFrames = -1;
	m_nRows = -1;
	m_nCols = -1;
	m_nImageID = -1;
}

void CCellAnimation::Init(int id, int width, int height, int maxFrames, int rows, int cols)
{
	m_nWidth = width;
	m_nHeight = height;
	m_nMaxFrames = maxFrames;
	m_nRows = rows;
	m_nCols = cols;
	m_nImageID = id;
	m_pTM = CSGD_TextureManager::GetInstance();
}

void CCellAnimation::Update(float fElapsed, CFrame& animInfo)
{
	//sanity check
	if( !animInfo.m_bIsPlaying )
		return;

	animInfo.m_fElapsed += fElapsed * animInfo.m_fSpeed;

	if( animInfo.m_fElapsed > animInfo.m_fTPerFrame )
	{
		animInfo.m_nCurrFrame++;
		animInfo.m_fElapsed = 0.0f;
	}

	if( animInfo.m_nCurrFrame >= m_nMaxFrames )
	{
		if( animInfo.m_bIsLooping )
			animInfo.m_nCurrFrame = animInfo.m_nStartFrame;
		else
		{
			animInfo.m_bIsPlaying = false;
			animInfo.m_nCurrFrame = m_nMaxFrames - 1;
		}
	}
}

void CCellAnimation::Render(int frame, int x, int y, float scalex, float scaley, DWORD dwColor, bool flipped, bool playing, bool looping)
{
	int localX = x - (int)CCamera::GetInstance()->OffsetX();
	int localY = y - (int)CCamera::GetInstance()->OffsetY();
	CGame* g = CGame::GetInstance();

	if( localX + (m_nWidth) < 0 || localX > g->GetWidth() || localY + (m_nHeight) < 0 || localY > g->GetHeight() )
		return;

	RECT srcTile = CellAlgorithm( frame );

	float XScale = scalex;

	if( flipped )
	{
		XScale = -scalex;
	}
	
	m_pTM->Draw( m_nImageID, localX, localY, XScale, scaley, &srcTile, 0, 0, 0, dwColor );
}
	
////////////////////////////////////////////////////////////
//Purpose:
//
//In:
//
//Return:
////////////////////////////////////////////////////////////
bool CCellAnimation::CheckCollision(CBaseObject* pBase, CFrame key)
{
	return false;
}

////////////////////////////////////////////////////////////
//Purpose:
//
//In:
//
//Return:
////////////////////////////////////////////////////////////
bool CCellAnimation::CheckCollision(CFrame* pBase, CFrame key)
{
	return false;
}


RECT CCellAnimation::CellAlgorithm(int id)
{
	RECT rCell;

	rCell.left		=	(id % m_nCols)		*	m_nWidth;
	rCell.top		=	(id / m_nCols)		*	m_nHeight;
	rCell.right		=	rCell.left			+	m_nWidth;
	rCell.bottom	=	rCell.top			+	m_nHeight;

	return rCell;
}