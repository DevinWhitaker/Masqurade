#include "AnchorAnimation.h"
#include "Camera.h"
#include "Game.h"
#include "BaseObject.h"
#include "AnimationManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../tinyxml/tinyxml.h"
#include <sstream>

////////////////////////////////////////////////////////////
//Purpose:	constructor
//
//In:		None
//
//Return:	None
////////////////////////////////////////////////////////////
CAnchorAnimation::CAnchorAnimation(void)
{
	m_nImageID = -1;
	m_pTM = nullptr;
	m_vAnchors.clear();
}


////////////////////////////////////////////////////////////
//Purpose:	destructor
//
//In:		None
//
//Return:	None
////////////////////////////////////////////////////////////
CAnchorAnimation::~CAnchorAnimation(void)
{
	for(size_t i = 0; i < m_vAnchors.size(); i++ )
	{
		tAnchor* temp = m_vAnchors[i];
		delete temp;
		m_vAnchors[i] = nullptr;
	}
	m_vAnchors.erase( m_vAnchors.begin(), m_vAnchors.end() );

	m_nImageID = -1;
	m_pTM = nullptr;
	m_vAnchors.clear();
}

////////////////////////////////////////////////////////////
//Purpose:	creates a tAnchor structure and adds it to the animation
//
//In:		
//			source:		the section of the image to draw when rendering this frame
//			anchor:		represents the actual position of the object
//			duration:	how much time to spend on this frame
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnchorAnimation::CreateAnchor(RECT source,POINT anchor, float duration, vector<AnimationEvent*> events)
{
	m_vAnchors.push_back( new tAnchor(source,anchor,duration,events) );
}

////////////////////////////////////////////////////////////
//Purpose:	sets the image id
//
//In:		the index into the texture manager that this
//			animation should render from
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnchorAnimation::Init(int id)
{
	m_nImageID = id;
	m_pTM = CSGD_TextureManager::GetInstance();
}

////////////////////////////////////////////////////////////
//Purpose:	draws this animation to the screen
//
//In:		
//		frame:		which frame this animation is currently on
//		x:			the position on the screen to draw the animation
//		y:			the position on the screen to draw the animation
//		scalex:		scales the width by this amount
//		scaley:		scales the height by this amount
//		dwColor:	tints the animation this color
//		flipped:	renders the animation backwards if true
//		playing:	renders the animation if true
//		looping:	NOT USED
//
//Return:
////////////////////////////////////////////////////////////
void CAnchorAnimation::Render(int frame, int x, int y, float scalex, float scaley, DWORD dwColor, bool flipped, bool playing, bool looping)
{
	int localX = x - (int)CCamera::GetInstance()->OffsetX();
	int localY = y - (int)CCamera::GetInstance()->OffsetY();
	RECT srcrect = m_vAnchors[frame]->Bounds;
	POINT srcanchor =  m_vAnchors[frame]->Anchor;
	int width = srcrect.right - srcrect.left;
	int height = srcrect.bottom - srcrect.top;
	CGame* g = CGame::GetInstance();

#if TRUE//_DEVIN
	int templocX = localX;
	int templocY = localY;
#endif

	if( localX + (width) < 0 || localX > g->GetWidth() || localY + (height) < 0 || localY > g->GetHeight() )
		return;

	float XScale = scalex;

	if(flipped)
	{
		XScale = -scalex;
		localX += int(width * scalex);
	}
	
	m_pTM->Draw( m_nImageID, (int)(localX - (srcanchor.x * scalex)), (int)(localY - (srcanchor.y * scaley)), XScale, scaley, &srcrect, 0, 0, 0, dwColor );

#if FALSE//_DEVIN
	RECT real;
	SetRect(&real,templocX+m_vAnchors[frame].Anchor.x,templocY+m_vAnchors[frame].Anchor.y,(templocX+width)-m_vAnchors[frame].Anchor.x,(localY+height)-m_vAnchors[frame].Anchor.y);
	CSGD_Direct3D::GetInstance()->DrawRect( real, 255, 255, 0 );
#endif
}

////////////////////////////////////////////////////////////
//Purpose:	determines which frame to play next
//
//In:
//		fElapsed:	change in time
//		animInfo:	structure that stores and updates the animations
//					updateble info
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnchorAnimation::Update(float fElapsed, CFrame& animInfo)
{
	//sanity check
	if( !animInfo.m_bIsPlaying )
		return;

	animInfo.m_fElapsed += fElapsed * animInfo.m_fSpeed;

	if( animInfo.m_fElapsed > m_vAnchors[animInfo.m_nCurrFrame]->Duration )
	{
		if( animInfo.m_bReverse )
			animInfo.m_nCurrFrame--;
		else
			animInfo.m_nCurrFrame++;
		animInfo.m_fElapsed = 0.0f;
	}

	if( (size_t)animInfo.m_nCurrFrame >= m_vAnchors.size() )
	{
		if( animInfo.m_bIsLooping )
		{
			if( animInfo.m_bReverse )
				animInfo.m_nCurrFrame = m_vAnchors.size() - 1;
			else
				animInfo.m_nCurrFrame = 0;
		}
		else
		{
			animInfo.m_bIsPlaying = false;
			animInfo.m_nCurrFrame = m_vAnchors.size() - 1;
		}
	}
}

////////////////////////////////////////////////////////////
//Purpose:	determine if a collision has happend
//
//In:
//		pBase:	object to check collision against
//		key:	the animation info of the object checking collision
//
//Return:	true if collision happend
////////////////////////////////////////////////////////////
bool CAnchorAnimation::CheckCollision(CBaseObject* pBase, CFrame key)
{
	bool value = false;
	LPRECT dest = nullptr;RECT animation;RECT rkey = key.GetRect();
	int count = m_vAnchors[key.m_nCurrFrame]->Events.size();
	vector<AnimationEvent*> events = m_vAnchors[key.m_nCurrFrame]->Events; 

	for(int i = 0; i < count; i++)
	{
		if(key.isFlipped())
		{
			int width = rkey.right - rkey.left;
			int ld = width - (rkey.left+events[i]->collision.left);
			int rd = width - (rkey.left+events[i]->collision.right);
			SetRect(&animation,
				(rkey.left+width)+rd,
				rkey.top+events[i]->collision.top,
				(rkey.left+width)+ld,
				rkey.top+events[i]->collision.bottom);
		}
		else
		{
			SetRect(&animation,
				rkey.left+events[i]->collision.left,
				rkey.top+events[i]->collision.top,
				rkey.left+events[i]->collision.right,
				rkey.top+events[i]->collision.bottom);
		}

		value = TRUE == (IntersectRect(dest,&animation,&pBase->GetRect()));
		if( value )
		{
			key.m_pOwner->CollisionResponse(pBase,*dest,events[i]->collision,m_vAnchors[key.m_nCurrFrame]->Anchor);
			if( events[i]->eventID != "NONE" )
			{
				CSGD_EventSystem::GetInstance()->QueueEvent(events[i]->eventID);
#if _DEVIN
				//printf(events[i]->eventID.c_str());
				//printf("\n");
#endif
			}
		}
	}
	return value;
}

////////////////////////////////////////////////////////////
//Purpose:	determine if a collision has happened
//
//In:
//		pBase:	the animation info of the object to check collision agains
//		key:	the animation info of the object checking collision
//
//Return:	true if collision happend
////////////////////////////////////////////////////////////
bool CAnchorAnimation::CheckCollision(CFrame* pBase, CFrame key)
{
	bool value = false;
	RECT dest;
	CAnimationManager* AM = CAnimationManager::GetInstance();

	CAnchorAnimation* my_animation = (CAnchorAnimation*)AM->operator[](key.GetAnimationID()); 
	tAnchor* my_anchor = my_animation->operator[](key.m_nCurrFrame);
	CAnchorAnimation* their_animation = (CAnchorAnimation*)AM->operator[](pBase->GetAnimationID()); 
	tAnchor* their_anchor = their_animation->operator[](pBase->m_nCurrFrame);

	//Need: their rect & their events, my rect & my events
	RECT us = key.GetRect();
	vector<AnimationEvent*> my_events = my_anchor->Events;
	RECT them = pBase->GetRect();
	vector<AnimationEvent*> their_events = their_anchor->Events;

	for(size_t i = 0; i < my_events.size(); i++)
	{
		RECT myCollision;

			SetRect(&myCollision,
				us.left+my_events[i]->collision.left,
				us.top+my_events[i]->collision.top,
				us.left+my_events[i]->collision.right,
				us.top+my_events[i]->collision.bottom);
		

		for(size_t j = 0; j < their_events.size(); j++)
		{
			RECT theirCollision;

				SetRect(&theirCollision,
					them.left+their_events[j]->collision.left,
					them.top+their_events[j]->collision.top,
					them.left+their_events[j]->collision.right,
					them.top+their_events[j]->collision.bottom);
			
#if _DEVIN
				CSGD_Direct3D::GetInstance()->DrawRect( myCollision, 255, 0, 0 );
				CSGD_Direct3D::GetInstance()->DrawRect( theirCollision, 255, 0, 0 );
#endif
			value = TRUE == IntersectRect(&dest,&myCollision,&theirCollision);

			if( value )
			{
				if( key.GetOwner()->GetType() == OBJ_PUZZLE )
					int x = 0;

				key.m_pOwner->CollisionResponse(pBase->GetOwner(),dest,myCollision,m_vAnchors[key.m_nCurrFrame]->Anchor);
				if( my_events[i]->eventID != "NONE" )
				{
					CSGD_EventSystem::GetInstance()->QueueEvent(my_events[i]->eventID);
#if _DEVIN
					printf(my_events[i]->eventID.c_str());
					printf("\n");
#endif
				}
			}
		}
	}
	return value;
}

void CAnchorAnimation::CreateFromFile( char* filename )
{
	TiXmlDocument doc;
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	if( doc.LoadFile( filename ) == false )
		return;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return;

	TiXmlElement* pAnimation = pRoot->FirstChildElement( "animation_info" );

	while( pAnimation != nullptr )
	{
		string imageFile = pAnimation->Attribute("image_file");
		string animationID = pAnimation->Attribute("name");

		int r = atoi(pAnimation->Attribute("r"));
		int g = atoi(pAnimation->Attribute("g"));
		int b = atoi(pAnimation->Attribute("b"));

		wostringstream oss;
		oss << imageFile.c_str();

		int id = pTM->LoadTexture( oss.str().c_str(), D3DCOLOR_XRGB(r,g,b) );



			Init(id);

			int count = atoi(pAnimation->Attribute("num_anchors"));
			TiXmlElement* pAnchor = pAnimation->FirstChildElement("anchor");
			for( int i = 0; i < count; i++ )
			{
				TiXmlElement* pRect = pAnchor->FirstChildElement("rect");
				RECT src;
				src.left = atoi(pRect->Attribute("left"));
				src.right = atoi(pRect->Attribute("right"));
				src.top = atoi(pRect->Attribute("top"));
				src.bottom = atoi(pRect->Attribute("bottom"));

				TiXmlElement* pPoint = pAnchor->FirstChildElement("point");
				POINT anchr;
				anchr.x = atoi(pPoint->Attribute("x"));
				anchr.y = atoi(pPoint->Attribute("y"));

				TiXmlElement* pDuration = pAnchor->FirstChildElement("duration");
				float duration = (float)atof(pDuration->Attribute("value"));

				vector<AnimationEvent*> events;

				int numEvents = atoi(pAnchor->Attribute("events"));
				TiXmlElement* pEvent = pAnchor->FirstChildElement("event");

				for(int i =0; i < numEvents; i++)
				{
					AnimationEvent* e = new AnimationEvent;

					int left	= atoi(pEvent->Attribute("left"));
					int right	= atoi(pEvent->Attribute("right"));
					int top		= atoi(pEvent->Attribute("top"));
					int bottom	= atoi(pEvent->Attribute("bottom"));

					SetRect(&e->collision,left,top,right,bottom);
					e->eventID = pEvent->Attribute("id");

					events.push_back(e);

					pEvent = pEvent->NextSiblingElement("event");
				}

				CreateAnchor(src,anchr,duration,events);

				pAnchor = pAnchor->NextSiblingElement("anchor");
			}

			pAnimation = pAnimation->NextSiblingElement("animation_info");
	}
}

RECT CAnchorAnimation::GetRect( int i )
{
	return m_vAnchors[i]->Bounds;
}